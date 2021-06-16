#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include"btree.h"

static GtkWidget *word;
static GtkWidget *word_add;
static GtkWidget *word_del;
static GtkWidget *result_search;
static GtkWidget *result_add;
static GtkWidget *result_del;
static GtkWidget *meaning;
static GtkWidget *meaning_add;
static GtkWidget *search_button;
static GtkWidget *add_button;
static GtkWidget *del_button;
static GtkWidget *hist_search;

BTA *root;
FILE *fp;

char *hist;

//===

// Here's functions of search feature.

char *get_word(char* input)
{
  char *output = (char*)calloc(1001, sizeof(char));
  int size;
  btsel(root, input, output, 1001*sizeof(char), &size);
  return output;
}

char *search_word(char* wrdinp)
{
  char *item;
  item = (char*)calloc(81, sizeof(char));
  fp = fopen("word_index.txt", "r");
  while (strcmp(wrdinp, item) > 0)
    fgets(item, 81, fp);
  *(item + strlen(item) - 1) = '\0';
  fclose(fp);
  return item;
}

void start_search(GtkWidget *search, gpointer data)
{
  char *wrdinp = (char*)calloc(81, sizeof(char)), *word_inp;
  int l = 0, r = 0;

  word_inp = (char*)calloc(81, sizeof(char));
  wrdinp = (char *)gtk_entry_get_text(GTK_ENTRY(word));
  
  word_inp = strdup(search_word(wrdinp));
  
  char buffer[1051], hist_buffer[1001];
  snprintf(buffer, sizeof(buffer), "Result :\n %s   :   %s", word_inp, get_word(word_inp));
  if (strlen(hist_buffer) == 0)
    {
      hist = strdup(word_inp);
    }
  else
    {
      if (strlen(word_inp) + strlen(hist) < 1000)
	{
	  l = strlen(hist);
	  r = l + strlen(word_inp);
	  for (int i = l; i <= r; i++)
	    {
	      *(hist + i) = *(word_inp + i - l);
	    };
	  printf("%s\n", hist);
	  *(hist + strlen(hist) + 1) = '\0';
	  *(hist + strlen(hist)) = '\n';
	}
    };
  snprintf(hist_buffer, sizeof(hist_buffer), "History :\n%s", hist);
  gtk_label_set_text(GTK_LABEL(result_search), buffer);
  gtk_label_set_text(GTK_LABEL(hist_search), hist_buffer);
}

static void search_feature(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window, *grid, *search;
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Search");
  gtk_window_set_default_size (GTK_WINDOW (window), 400, 400);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  word = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), word, 0, 0, 1, 1);
    
  search = gtk_button_new_with_label("Search");
  g_signal_connect(search, "clicked", G_CALLBACK(start_search), NULL);
  gtk_grid_attach(GTK_GRID(grid), search, 1, 0, 1, 1);

  result_search = gtk_label_new("Result :                                                                 ");
  gtk_grid_attach(GTK_GRID(grid), result_search, 0, 1, 1, 1);

  hist_search = gtk_label_new("history :");
  gtk_grid_attach(GTK_GRID(grid), hist_search, 2, 0, 10, 10);

  gtk_widget_show_all(window);
}

//===

// Here's functions of add feature.

int add_to_index(char *word, char *meaning)
{
  int size;
  char tmpmeaning[1001], tmpword[81];
  strcpy(tmpword, word);
  strcpy(tmpmeaning, meaning);
  printf("add_to_index\n%s : %s\n", tmpword, tmpmeaning);
  if (btins(root, tmpword, tmpmeaning, 1001*sizeof(char)))
    {
      printf("Failed to insert btree\n");
      return 0;
    }
  printf("Insert to btree successfull\n");
  fp = fopen("word_index.txt", "w");
  btsel(root, "", tmpmeaning, 1001*sizeof(char), &size);
  while (!btseln(root, tmpword, tmpmeaning, 1001*sizeof(char), &size))
    fprintf(fp, "%s\n", word);
  printf("Before close file\n");
  fclose(fp);
  printf("finished add_to_index\n");
  return 1;
}

void start_add(GtkWidget *search, gpointer data)
{
  char *word, *meaning;
  printf("start_add\n");
  word = (char*)calloc(81, sizeof(char));
  meaning = (char*)calloc(1001, sizeof(char));
  word = (char*)gtk_entry_get_text(GTK_ENTRY(word_add));
  meaning = (char*)gtk_entry_get_text(GTK_ENTRY(meaning_add));
  printf("%s, %s\n", word, meaning);
  
  char buffer[150];
  if (add_to_index(word, meaning))
    snprintf(buffer, sizeof(buffer), "Status : Added successfully                                                                                 ");
  else
    snprintf(buffer, sizeof(buffer), "Status : Add failed\n - word existed or something gone wrong !                                                 ");
  gtk_label_set_text(GTK_LABEL(result_add), buffer);
}

static void add_feature(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window, *grid, *add, *word_label, *meaning_label;
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Add new word");
  gtk_window_set_default_size (GTK_WINDOW (window), 370, 200);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  word_label = gtk_label_new("Word : ");
  gtk_grid_attach(GTK_GRID(grid), word_label, 0, 1, 5, 1);
  word_add = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), word_add, 0, 2, 5, 1);

  meaning_label = gtk_label_new("Meaning : ");
  gtk_grid_attach(GTK_GRID(grid), meaning_label, 0, 3, 5, 1);
  meaning_add = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), meaning_add, 0, 4, 5, 1);
    
  add = gtk_button_new_with_label("Add");
  g_signal_connect(add, "clicked", G_CALLBACK(start_add), NULL);
  gtk_grid_attach(GTK_GRID(grid), add, 0, 0, 5, 1);

  result_add = gtk_label_new("Status :                                                                                                                             ");
  gtk_grid_attach(GTK_GRID(grid), result_add, 0, 5, 5, 1);

  gtk_widget_show_all(window);
}

//===

// Here's functions of delete feature.

void start_delete(GtkWidget *search, gpointer data)
{
  char *wtd = (char*)calloc(81, sizeof(char));
  char tmpmeaning[1001], tmpword[81];
  int size;
  wtd = (char *)gtk_entry_get_text(GTK_ENTRY(word_del));
  
  char buffer[150];

  if (!btdel(root, wtd))
    {
      fp = fopen("word_index.txt", "w");
      btsel(root, "", tmpmeaning, 1001*sizeof(char), &size);
      while (!btseln(root, tmpword, tmpmeaning, 1001*sizeof(char), &size))
	fprintf(fp, "%s\n", tmpword);
      fclose(fp);
      snprintf(buffer, sizeof(buffer), "Status : Deleted successfully                                                                           ");
    }
  else snprintf(buffer, sizeof(buffer), "Status : Delete failed\n - word is not existed or something gone wrong !                                ");

  gtk_label_set_text(GTK_LABEL(result_del), buffer);
}

static void del_feature(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window, *grid, *delete;
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Delete");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 100);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  word_del = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), word_del, 0, 0, 1, 1);
    
  delete = gtk_button_new_with_label("Delete");
  g_signal_connect(delete, "clicked", G_CALLBACK(start_delete), NULL);
  gtk_grid_attach(GTK_GRID(grid), delete, 4, 0, 1, 1);

  result_del = gtk_label_new("Status :                                                                                                                             ");
  gtk_grid_attach(GTK_GRID(grid), result_del, 0, 1, 1, 1);

  gtk_widget_show_all(window);
}

//===

int main(int argc, char **argv) {
  GtkWidget *window, *grid, *lot0, *lot1, *lot2, *lot3, *lot4, *lot5;
  gtk_init(&argc, &argv);
  //char *hist;
  hist = (char*)calloc(81, sizeof(char));
  
  root = btopn("index.dat", 0, 0);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_window_set_title (GTK_WINDOW (window), "Dictionary");
  gtk_window_set_default_size (GTK_WINDOW (window), 300, 170);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  lot0 = gtk_label_new("     ");
  gtk_grid_attach(GTK_GRID(grid), lot0, 0, 0, 1, 1);
  
  lot1 = gtk_label_new("                                ");
  gtk_grid_attach(GTK_GRID(grid), lot1, 0, 1, 1, 1);
  
  search_button = gtk_button_new_with_label("Search");
  g_signal_connect(search_button, "clicked", G_CALLBACK(search_feature), NULL);
  gtk_grid_attach(GTK_GRID(grid), search_button, 1, 1, 1, 1);

  lot2 = gtk_label_new("     ");
  gtk_grid_attach(GTK_GRID(grid), lot2, 0, 2, 1, 1);

  lot3 = gtk_label_new("                                ");
  gtk_grid_attach(GTK_GRID(grid), lot3, 0, 3, 1, 1);
  
  add_button = gtk_button_new_with_label("Add new word");
  g_signal_connect(add_button, "clicked", G_CALLBACK(add_feature), NULL);
  gtk_grid_attach(GTK_GRID(grid), add_button, 1, 3, 1, 1);

  lot4 = gtk_label_new("     ");
  gtk_grid_attach(GTK_GRID(grid), lot4, 0, 4, 1, 1);
  
  lot5 = gtk_label_new("                                ");
  gtk_grid_attach(GTK_GRID(grid), lot5, 0, 5, 1, 1);
  
  del_button = gtk_button_new_with_label("Delete a word");
  g_signal_connect(del_button, "clicked", G_CALLBACK(del_feature), NULL);
  gtk_grid_attach(GTK_GRID(grid), del_button, 1, 5, 1, 1);
  
  gtk_widget_show_all(window);
  gtk_main();

  btcls(root);
  return 0;
}
