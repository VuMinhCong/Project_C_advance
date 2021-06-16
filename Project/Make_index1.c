#include<stdlib.h>
#include"btree.h"
#include<string.h>

int main()
{
  BTA *root;
  FILE *fp;
  int k = 0, r, l;
  long x = 0;
  char inp[230], meaning[230], word[81], tmpmeaning[230];
  if ((root = btopn("index.dat", 0, 0)) == NULL)
    {
      root = btcrt("index.dat", 0, 0);
    }
  fp = fopen("index.txt", "r");
  
  fscanf(fp, "@%[^/]%s", word, tmpmeaning);
  fgetc(fp);
  word[strlen(word) - 1] = '\0';
  strcpy(meaning, tmpmeaning);
  meaning[strlen(tmpmeaning)] = '\n';
  meaning[strlen(tmpmeaning) + 1] = '\0';
  
  while (!feof(fp))
    {
      inp[0] = '\0'; 
      fgets(inp, 230, fp);
      x++;
      //printf("%ld\n", x);
      if (inp[0] == '\n')
	{
	  btins(root, strdup(word), strdup(meaning), (strlen(meaning)+1)*sizeof(char));
	  printf("%s : %s", word, meaning);
	  word[0] = '\0';
	  meaning[0] = '\0';
	  tmpmeaning[0] = '\0';
	  
	  fscanf(fp, "@%[^/]%s", word, tmpmeaning);
	  fgetc(fp);
	  
	  word[strlen(word) - 1] = '\0';
	  strcpy(meaning, tmpmeaning);
	  meaning[strlen(tmpmeaning)] = '\n';
	  meaning[strlen(tmpmeaning) + 1] = '\0';
	}
      else
	{
	  tmpmeaning[0] = '\0';
	  strcpy(tmpmeaning, inp);
	  if ((strlen(meaning) + strlen(tmpmeaning)) < 230)
	    {
	      l = strlen(meaning);
	      r = l + strlen(tmpmeaning);
	      for (int i = l; i <= r; i++)
		{
		  meaning[i] = tmpmeaning[i - l];
		};
	    }
	};
    };
  btcls(root);
  fclose(fp);
}
