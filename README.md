# Image and Video Steganography using LSB Algorithm with a variation

In this project, I have tried to implement Image and video steganography using a variation of LSB algorithm. In LSB algorithm, the least significant bit of color code of the pixel is modified to store the message. For example, if we want to store 'A' in the image, then we will find the binary equivalent of its ascii and the generated 8 bits which will be replaced by the 8 bits of the color codes of pixels of the image. Refer follow figure. 

<p align="center">
<img src="https://github.com/PratikMahajan50/ImageAndVideoSteganography/blob/main/LSBAlgo.png" width="450">
</p>

In this project, rather than storing the bits in the continuous manner, I have created an algorithm that will store the values in some pattern and generate a key which can be used to decode the message back. Algorithm is explained below.
So far I have completely implemented the Image steganography part using C++ along with the OpenCV library. The video steganography is yet to be implemented.
## Algorithm to hide the data in image

1. Find out the occurences of all the color codes(0-255) for each color channel(BGR) by scanning through all the pixels of the image and store their frequency in an array(count[3][255]).
2. Find out the length of the message to be stored, and store in a varible(mLen).
3. Now from the count array, find out the color codes whose frequency count is greater than or equal to the length of the message and store them in an array of structure which stores the color code and the color channel(eVals).
4. Now from this array of structure eVals, select any 1 value randomly. Now the color code of this selected structure will be the target code(tCode) and its color channel will be reference channel(rfCh).
5. Randomly select a change channel(chCh) which should be different from the reference channel.
6. Convert the message from string of characters to binary array.
7. Scan the image pixel by pixel. If the color code of the reference channel of the current pixel matches the calculated color code(tCode), then make the change in the color code of the change channel. Following is the logic to append the data using lsb technique
    - If the next value of the message to be store is 0 then we check if the color code that we are operating on is even 
      or odd. If it is odd than do -1 to make it even and thus the lsb bit becomes 0.
    - same if the next value of the message is 1, then we check if the color code we are operating is even or odd. If is 
      even, then add 1 to make the lsb bit 1.
8. Create a key that stores refCh, chCh, mLen and tCode.

## Algorithm to retrieve the data from the image

1. Use the genearated key to retrieve the required parameteres.
2. Scan the image pixel by pixe. If the color code of reference channel of current pixel matches the color code retrieved by the key, then store the lsb of that code in an array(bArray).
3. Convert the binary array to integer array of ascii and convert this array to string of characters.

## Algorithm to hide data in Video

1. Find out the number of bits to be stored in the video(length_of_message X 8).
2. Find out the number of bits to be stored in single frame(bits_length/no_of_frames).
3. Traverse through the first 5 frames of the video to find out the dominant/storable reference channel and code. Randomly choose the change channel among the rest.
4. Store the bits in all the frames using the reference channel and change channel. This can be done using multithreading on multiple threads simultaneously.
5. Generate the key accordingly.

## Algorithm to retrieve the data from the image

1. Use the genearated key to retrieve the required parameteres.
2. Retrieve the data from the frames using the same technique used to extract data from image in above algorithm.
3. Convert binary bits to the string of characters.


