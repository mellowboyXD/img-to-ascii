#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <stb_image_resize.h>

#include "ascii.h"

void frame2ascii(unsigned char *buf, int wrap, int w, int h)
{
	unsigned char *pixels = malloc(w * h);
	if (!pixels) {
		fprintf(stderr, "Could not malloc pixels for resizing\n");
		return;
	}

	memcpy(pixels, buf, w * h);

	int desired_w = 80;
	int desired_h = (((float)desired_w / w) * h) / 2;
	int new_wrap = ((float)wrap / w) * desired_w;
	stbir_resize_uint8(pixels, w, h, 0, pixels, desired_w, desired_h, 0, 1);

	for (int i = 0; i < desired_w * desired_h; ++i) {
		int idx = i;
		char c = map_brightness_to_char(pixels[idx]);
		printf("%c", c);
		if (i % new_wrap == 0)
			printf("\n");
	}
	printf("\n");
	free(pixels);
}

void ansi_clear()
{
	printf("\033[H\033[2j");
	printf("\n");
}

int decode_packet(AVPacket *packet, AVCodecContext *codecctx, AVFrame *frame)
{
	int res = avcodec_send_packet(codecctx, packet);

	if (res < 0) {
		fprintf(stderr, "Error sending packet to decoder: %s\n",
			av_err2str(res));
		return res;
	}

	while (res >= 0) {
		res = avcodec_receive_frame(codecctx, frame);
		if (res == AVERROR(EAGAIN) || res == AVERROR_EOF) {
			break;
		} else if (res < 0) {
			fprintf(stderr,
				"Error receiving frames from decoder: %s\n",
				av_err2str(res));
			return res;
		}
		ansi_clear();
		frame2ascii(frame->data[0], frame->linesize[0], frame->width,
			    frame->height);
	}

	return 0;
}

int main(int argc, const char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Video file not provided\n");
		return -1;
	}

	int exit_status = 0;
	const char *filepath = argv[1];

	AVFormatContext *formatCtx = avformat_alloc_context();
	if (!formatCtx) {
		fprintf(stderr, "Error allocating AVFormatContext\n");
		exit_status = 1;
		goto video_cleanup;
	}

	printf("Opening input file and loading format header\n");

	if (avformat_open_input(&formatCtx, filepath, NULL, NULL) != 0) {
		fprintf(stderr, "Error opening input filr\n");
		exit_status = 1;
		goto video_cleanup;
	}

	printf("Finding stream info from format\n");

	if (avformat_find_stream_info(formatCtx, NULL) < 0) {
		fprintf(stderr, "Error finding stream info\n");
		exit_status = 1;
		goto video_cleanup;
	}

	printf("Fmt: %s duration: %ld us bit rate: %ld\n",
	       formatCtx->iformat->name, formatCtx->duration,
	       formatCtx->bit_rate);

	const AVCodec *codec = NULL;
	AVCodecParameters *codecparams = NULL;
	int video_stream_index = -1;

	for (unsigned int i = 0; i < formatCtx->nb_streams; ++i) {
		AVCodecParameters *lcodecparams = NULL;
		const AVCodec *lcodec = NULL;

		lcodecparams = formatCtx->streams[i]->codecpar;

		printf("finding proper decoder\n");

		lcodec = avcodec_find_decoder(lcodecparams->codec_id);
		if (!lcodec) {
			fprintf(stderr, "Error: unsupported codec\n");
			continue;
		}

		if (lcodecparams->codec_type == AVMEDIA_TYPE_VIDEO) {
			if (video_stream_index == -1) {
				video_stream_index = i;
				codec = lcodec;
				codecparams = lcodecparams;
			}

			printf("Video codec: resolution %d x %d\n",
			       lcodecparams->width, lcodecparams->height);
			// TODO: Handle audio type
		}

		printf("Codec %s ID %d bit_rate %ld\n", lcodec->name,
		       lcodec->id, lcodecparams->bit_rate);
	}

	if (video_stream_index == -1) {
		fprintf(stderr, "File does not contain video stream\n");
		exit_status = -1;
		goto video_cleanup;
	}

	AVCodecContext *codecctx = avcodec_alloc_context3(codec);
	if (!codecctx) {
		fprintf(stderr, "failed to allocate context\n");
		exit_status = -1;
		goto video_cleanup;
	}

	if (avcodec_parameters_to_context(codecctx, codecparams) < 0) {
		fprintf(stderr, "failed to copy parameters to context\n");
		exit_status = -1;
		goto video_cleanup;
	}

	if (avcodec_open2(codecctx, codec, NULL) < 0) {
		fprintf(stderr, "failed to open codec through avcodec_open2\n");
		exit_status = -1;
		goto video_cleanup;
	}

	AVFrame *frame = av_frame_alloc();
	if (!frame) {
		fprintf(stderr, "error allocating memory for frame\n");
		exit_status = -1;
		goto video_cleanup;
	}

	AVPacket *packet = av_packet_alloc();
	if (!packet) {
		fprintf(stderr, "failed to allocate packet\n");
		exit_status = -1;
		goto video_cleanup;
	}

	int res = 0;
	int64_t duration = formatCtx->duration;

	while (av_read_frame(formatCtx, packet) >= 0) {
		if (packet->stream_index == video_stream_index) {
			double curr_time =
				packet->pts * AV_TIME_BASE *
				av_q2d(formatCtx->streams[video_stream_index]
					       ->time_base);
			res = decode_packet(packet, codecctx, frame);
			printf("Progress: %.2f%%\n",
			       (curr_time / duration) * 100);
			if (res < 0) {
				break;
			}
		}
		// reset packet to take new packets
		av_packet_unref(packet);
	}

video_cleanup:
	printf("Releasing all resources\n");
	avformat_free_context(formatCtx);
	avcodec_free_context(&codecctx);
	av_frame_free(&frame);
	av_packet_free(&packet);
	return exit_status;
}
