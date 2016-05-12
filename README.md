###Description
Indexer reads in the HTML files downloaded by crawler and parses them to create an index of words and their frequencies. 
The index is stored in a file called index.dat, which lists the words along with an identifier to the files they were found 
in, and their frequency in said file. The program has two modes: one is a testing mode that allows the user to test index writing
and reading functionality.

###Running indexer
**running normally**<br>
Command Input<br>
``./indexer [TARGET_DIRECTORY] [RESULTS FILENAME]``<br>
Example command input<br>
``./indexer ./data/ index.dat``

**Running index test version**<br>
Command Input<br>
``./indexer [TARGET_DIRECTORY] [RESULTS FILENAME] [REWRITTEN FILENAME]``<br>
Example command input<br>
``./indexer ./data/ index.dat new_index.dat``

Further documentation can be found [here](./Indexer documentation)
