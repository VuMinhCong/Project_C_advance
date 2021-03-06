#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include"btree.h"

static GtkWidget *word;
static GtkWidget *word_add;
static GtkWidget *result_search;
static GtkWidget *result_add;
static GtkWidget *meaning;
static GtkWidget *meaning_add;
static GtkWidget *search_button;
static GtkWidget *add_button;

BTA *root;
FILE *fp;

//===

// Here's functions of search feature.

char *get_word(char* input)
{
  char *output = (char*)calloc(231, sizeof(char));
  int size;
  btsel(root, input, output, 231*sizeof(char), &size);
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
  word_inp = (char*)calloc(81, sizeof(char));
  wrdinp = (char *)gtk_entry_get_text(GTK_ENTRY(word));
  
  word_inp = strdup(search_word(wrdinp));
  
  char buffer[100];
  snprintf(buffer, sizeof(buffer), "Result :\n %s   :   %s", word_inp, get_word(word_inp));

  gtk_label_set_text(GTK_LABEL(result_search), buffer);
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
  gtk_grid_attach(GTK_GRID(grid), search, 4, 0, 1, 1);

  result_search = gtk_label_new("");
  gtk_grid_attach(GTK_GRID(grid), result_search, 0, 1, 1, 1);

  gtk_widget_show_all(window);
}

//===

// Here's functions of add feature.

int add_to_index(char *word, char *meaning)
{
  int size;
  char tmpmeaning[231], tmpword[81];
  strcpy(tmpword, word);
  strcpy(tmpmeaning, meaning);
  printf("add_to_index\n%s : %s\n", tmpword, tmpmeaning);
  fp = fopen("word_index.txt", "w");
  if (btins(root, word, meaning, 231*sizeof(char)))
    {
      printf("Failed to insert btree\n");
      return 0;
    }
  printf("Insert to btree successfull\n");
  btsel(root, "", tmpmeaning, 231*sizeof(char), &size);
  while (!btseln(root, tmpword, tmpmeaning, 231*sizeof(char), &size))
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
  meaning = (char*)calloc(231, sizeof(char));
  word = (char*)gtk_entry_get_text(GTK_ENTRY(word_add));
  meaning = (char*)gtk_entry_get_text(GTK_ENTRY(meaning_add));
  printf("%s, %s\n", word, meaning);
  //wrdinp = (char *)gtk_entry_get_text(GTK_ENTRY(word));
  
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
  gtk_window_set_default_size (GTK_WINDOW (window), 370, 400);

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

int main(int argc, char **argv) {
  GtkWidget *window, *grid, *lot1, *lot2, *lot3;
  gtk_init(&argc, &argv);
  
  root = btopn("index.dat", 0, 0);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_window_set_title (GTK_WINDOW (window), "Dictionary");
  gtk_window_set_default_size (GTK_WINDOW (window), 300, 200);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  lot1 = gtk_label_new("                                ");
  gtk_grid_attach(GTK_GRID(grid), lot1, 0, 0, 1, 1);
  
  search_button = gtk_button_new_with_label("Search");
  g_signal_connect(search_button, "clicked", G_CALLBACK(search_feature), NULL);
  gtk_grid_attach(GTK_GRID(grid), search_button, 1, 0, 1, 1);

  lot2 = gtk_label_new("     ");
  gtk_grid_attach(GTK_GRID(grid), lot2, 0, 1, 1, 1);

  lot3 = gtk_label_new("                                ");
  gtk_grid_attach(GTK_GRID(grid), lot3, 0, 2, 1, 1);
  
  add_button = gtk_button_new_with_label("Add new word");
  g_signal_connect(add_button, "clicked", G_CALLBACK(add_feature), NULL);
  gtk_grid_attach(GTK_GRID(grid), add_button, 1, 2, 1, 1);
  
  gtk_widget_show_all(window);
  gtk_main();

  btcls(root);
  return 0;
}
