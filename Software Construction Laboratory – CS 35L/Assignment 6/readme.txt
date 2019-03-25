I used “wget https://web.cs.ucla.edu/classes/winter19/cs35L/assign/srt.tgz” to
download Brian Allen’s SRT implementation. To unzip the tar file srt.tgz, I
ran “tar xvf srt.tgz”.

The first difficulty I faced was figuring out which code could be distributed
across multiple processors through threading. I knew that the main function is
called first in the code, so I started there.

Int main(): I changed the if statement to allow the argument input of the
number of threads to be greater than 1. Every time a function was called, I
checked to see if the definition was in the main.c file. If it was, I checked
to see if that function was able to be run by multiple threads simultaneously
(i.e. create_sphereflake_scene). In the case of create_sphereflake_scene,
there was no need to do threading because we were only assigning a value to
one variable. Multiple threads would have simply kept reassigning the same
value and actually would have caused garbage extra memory allocation.  I knew
I had to find a way to distribute the workload of the large for loop function
in int main(). I created an array of threads with matching thread ID’s and
moved the for loop to a function I created because I knew pthread_create(args)
needs a function as one of its inputs.

Static void* picSplit (void* threadNum): This is the function I created out of
the for loop from int main(). Since each thread needed to evaluate different
pixels of the resultant image, I created simple math algorithms to decipher
what the start and end points would be for each thread. For example, if
“width” was set to 1000 and the number of threads was 4, each thread would
take on 250 iterations of the outer for loop.  I noticed that I did not have
to return anything because picSplit was only changing existing values.

What was very difficult about this function, which took me a majority of my
time, was converting the values that were printed into a data structure that I
could print all at once at the end of my int main() function. I ended up doing
this by defining a struct with three values, and then creating a 2D array of
this struct. Each element of this 2D array mapped to a pixel value (x,y) with
three colors (defined by the struct).

Conclusion: This was the output when I ran “make clean check”:

For 1 thread real 0m41.285s user 0m41.278s sys 0m0.000s

For 2 threads real 0m22.000s user 0m43.223s sys 0m0.004s

For 4 threads real 0m15.013s user 0m44.037s sys 0m0.002s

For 8 threads real 0m9.209s user 0m44.060s sys 0m0.005s

Clearly, the speed increases greatly as we increase the number of
threads. However, the rate of increase of efficiency decreases as we increase
the number of threads. For example, the difference between 4 and 8 threads is
6 seconds, while that of 1 and 2 threads is approximately 19 seconds. This
pattern continues as we approach the total number of threads provided by the
UCLA linux server.
