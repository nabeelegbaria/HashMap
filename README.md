# HashMap
The HashMap we create is template one , we implemented the insert,erase,contain,at,clear,resize ... functions
which are used to update the HashMap.

in addition we implemented the [],=,==,!= operators , and an inner class forward const iterator .

the program receives two files , we put the first file in a hashmap , 
the second file is text file , 
for each line exists in the first file we have to count how many times
they appeared in the second file , for this job we created a count hash , 
at the end , we calculated the score , if the score is bigger than the given threshold 
a SPAM will be printed , NOT_SPAM otherwise .
we considered the wrong cases , such as negative threshold , negative numbers , empty lines or given out of range factors .

the resize function is used when the current loadfactor is bigger than the upper factor or lower than the lower factor , 
in such case , we multiply the capacity by 2 or divide it by 2.

there were some functions that were implemented twice , such as the operator [] , once for reading without writing,
and the other is for both writing and reading .

we implemented a constructor which receives two vectors , creates a HashMap from them , so the pair first data member 
is taken from the first vector, the second data member is taken from the second vector.
the table of the hash is represented by HashTable data member , which is a vector of vectors of pairs .
