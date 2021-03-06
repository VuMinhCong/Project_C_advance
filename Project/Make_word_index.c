#include<stdlib.h>
#include"btree.h"

int main()
{
  BTA *root;
  char word[81], meaning[230];
  FILE *fp;
  fp = fopen("word_index.txt", "w");
  root = btopn("index.dat", 0, 0);
  btsel(root, "", meaning, 230*sizeof(char), &size);
  while (!btseln(root, word, meaning, 230*sizeof(char), &size))
    fprintf(fp, "%s\n", word);
  fclose(fp);
  btcls(root);
}
