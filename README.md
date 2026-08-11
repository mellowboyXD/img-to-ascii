<img width="733" height="462" alt="image" src="https://github.com/user-attachments/assets/81221577-96db-4414-a3da-a61ec96311f9" />


PLAN
	* Convert to grayscale
	* Calculate brightness
	* Map ASCII
	* Assemble
	* Did some refactoring
	* Fix aspect ratio bug
		now rendered ascii is proportionately correct
	* Refactor
	* Parse cmd options -w/--width to set output width
	* Parse output file
		now has option to output to file instead of stdout

	+ Parse video file to convert to ascii

	x Divide into blocks
		- Scrapped this idea. Tried to calculate the average brightness 
		  per block but ended up just loosing too much info about each 
		  pixel.
		- Instead, I resized the image to match the desired width using 
		  the `stb_image_resize` library.

COMPILE
	Simply run `make`.
	Run `./bin/img2ascii <image_file>.png`

DEPENDENCIES
	`stb_image` is a simple image processing library.

REFERENCES
	`pixcii` - https://github.com/ashish0kumar/pixcii/
	`img2ascii` - https://github.com/JosefVesely/img2ascii/
