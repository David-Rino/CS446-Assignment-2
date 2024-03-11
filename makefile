looped_sum: looped_sum.c
	gcc -o looped_sum looped_sum.c -Wall

threaded_sum: threaded_sum.c
	gcc -o threaded_sum threaded_sum.c -Wall