# PNG Finder/Concatenator

<h2> Purpose: </h2>

<p>
  findpng.c: This is a C program that searches through a provided directory, and finds all legitimate files that are in the PNG file format.
  
  catpng.c: This is a C program that utilizes the underlying structure of PNG file format to concatenate (combine) various PNG files into a single PNG file called "all.png".
</p>

<h2> How it works </h2>

<p>
  findpng.c: Uses iteration and the fread() function to read the header (first 8 bytes) of each file under a directory. If the second, third, and fourth byte correspond to the ASCII code of 'P', 'N', 'G' respectively, then this is a legitimate PNG file.
  
  catpng.c:  The general process of PNG concatenation involves retrieving the filtered pixel data of each image, then concatenate the two chuncks of filtered pixel data arrays vertically. Then we apply 
</p>
