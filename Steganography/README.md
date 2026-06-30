## Description
LSB (Least Significant Bit) Image Steganography is a C programming project that demonstrates how secret information can be securely hidden inside a bitmap (.bmp) image using the Least Significant Bit (LSB) technique.

The project embeds the contents of a secret text file into the least significant bits of the image pixels without causing noticeable changes to the image quality. The resulting image, known as the stego image, appears visually identical to the original image while containing the hidden information.

The decoding module extracts the embedded data from the stego image by reversing the encoding process, ensuring accurate and reliable recovery of the original secret message.

## Features
- Encode a secret text file into a BMP image.
- Decode the hidden data from the stego image.
- Preserves the visual quality of the original image.
- Supports secure data hiding using the LSB technique.
- Simple command-line interface.

## Technologies Used
- C Programming
- File Handling
- Bit Manipulation
- Bitmap (BMP) Image Processing

## Compilation

Compile all source files using:
 gcc *.c

 ## Execution
### Encoding
./a.out -e image.bmp secret.txt stego.bmp

#### Arguments:

- -e – Encode mode
- image.bmp – Cover image used to hide the data
- secret.txt – File containing the secret message
- stego.bmp - Image with hidden message

### Decoding
./a.out -d stego.bmp output.txt

#### Arguments:

-d – Decode mode
stego.bmp – Image containing the hidden message
output.txt – Output file where the decoded message is stored

## Project Workflow
- Read the secret text file.
- Embed the secret data into the least significant bits of the BMP image.
- Generate the stego image.
- Decode the hidden data from the stego image.
- Store the extracted message in the output text file.
