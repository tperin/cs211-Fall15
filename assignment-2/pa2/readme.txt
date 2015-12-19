I used a trie as the data structure to store dictionary words. Then, the script parses out words from the data file, and for each word it traverses the tree until it either finds the word or hits a null child, meaning there's no matching word.

In the first part, I stored wordcount and prefix count for each node. The second one just stores word count, and calculates prefix count as it traverses the trie. The writing is done recursively.

The dictionary words adding to the trie is the shorter part, running at O(n*k). The data search is the longest part of the script, running at O(m*k). The space complexity is O(n*k), since words are only stored once and have a linked list of the number of characters. 
