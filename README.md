# One-Shot-Learning
Learn is a project I made for Computer Architecture that takes in data regarding attributes and prices of a given number of houses, then uses that information to predict the price of another house based on the house's attributes.  I particularly like this project because it is a very simplified imitation of machine learning.  The program examines a set of data, and learns how to assess other data from the given resources.

Included are two folders: /src and /data.  /src contains the relevant C file, learn.c, and a makefile.  /data contains several instances of data that can be used to display the program at work.

In the data folder, there are three types of files: training files, data files, and reference files.  Upon compiling the program, a training file should be passed first, followed by a data file.  A reference file can then be examined to see if the output from the program is correct.  Training files take the form "test.XX.train.txt", data files take the form "test.XX.data.txt", and reference files take the form "test.XX.ref.txt", with XX being a two-digit ID number for a specific test.

Example: "/learn test.00.train.txt test.00.data.txt" from the command line will produce output equivalent to the text file "test.00.ref.txt".
