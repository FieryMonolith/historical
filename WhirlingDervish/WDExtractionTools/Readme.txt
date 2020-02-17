These three perl scripts can be used to extract individuals from 
the output data file generate by WhirlingDervish.

get_best.pl [filename] [prefix]* :

Gets the best individual in the final generation.  The prefix will 
be prepended to the beginning of the output file name, but it is 
optional.


get_generation.pl [filename] [generation number] :

Get all the individuals in the given generation.  Each individual 
is output to its own file with the name generation_id_fitness.cre.


get_last_generation.pl [filename] :

Gets all the individuals in the final generation by first finding the 
last generation and then calling get_generation.pl.
