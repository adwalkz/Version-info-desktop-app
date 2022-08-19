
#include<gtk/gtk.h>
#include<stdlib.h>

//_________________________________________________________________________________________________

GtkWidget *window;
GtkWidget *workArea;

GtkWidget *nb;
GtkWidget *textArea;
GtkWidget *scrlBox;

GtkTextBuffer *buf;
GtkTextIter head, tail;

gint width, height;
int i=0;
char file[5][20] = {"UserInfo", "SystemInfo", "HardwareInfo", "MemoryInfo", "OSInfo"};

//_________________________________________________________________________________________________

/* This helping function 
which resize widget childs*/
void resize()
{
	gtk_window_get_size(GTK_WINDOW(window), &width, &height);
	gtk_widget_set_size_request(scrlBox, width-3, height-40);
	gtk_widget_queue_draw(window);
	gtk_widget_show_all(window);
}

/* This helping function 
which add new tab*/
void gtk_add_page_new()
{
	textArea = gtk_text_view_new();

	buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textArea));
	//gtk_text_buffer_set_text(buf, "", gtk_text_buffer_get_char_count(buf));

	scrlBox = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(scrlBox), textArea);

	gtk_notebook_append_page(GTK_NOTEBOOK(nb), scrlBox, NULL);
	//gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK(nb), scrlBox, TRUE);
	//gtk_widget_grab_focus(textArea);

	gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(nb), scrlBox, file[i]);
}

void task()
{
	/*for(i=0;i<6;i++)
	{
		if(i<5)
			gtk_add_page_new();
		if(i==0)
			system("who > SampleText");
		if(i==1)
			system("uname -a > SampleText");
		if(i==2)
			system("lshw -short > SampleText");
		if(i==3)
			system("vmstat -s > SampleText");
		if(i==4)
			system("lsb_release -a > SampleText");
		
		FILE *fp = fopen("SampleText", "r");
		if(fp == NULL)
			printf("NOT FOUND\n");
		else
		{
			fseek(fp,0,SEEK_END);
			int size = ftell(fp);
			fseek(fp,0,SEEK_SET);
			char *t = malloc(size);
			int ret = fread(t, 1, size, fp);
			gtk_text_buffer_set_text(buf, t, ret);
			free(t);
		}
		fclose(fp);
	}
	gtk_widget_show_all(window);
	system("rm SampleText");*/
	
	int ret;
	char com[5][20] = {"who", "uname -a", "lshw -short", "vmstat -s", "lsb_release -a"};
	char path[PATH_MAX];
	FILE *fp;

	for(i=0;i<5;i++)
	{
		fp = popen(com[i], "r");
		
		gtk_add_page_new();

		if(fp == NULL)
			printf("FILE NOT OPEN\n");
		else
		{
			ret = fread(path, 1, PATH_MAX, fp);
			gtk_text_buffer_set_text(buf, path, ret);
			pclose(fp);
		}
	}
}

//_________________________________________________________________________________________________

int main()
{
	gtk_init(NULL, NULL);
	GtkCssProvider *css;
//-------------------------------------------------------------------------------------------------
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Version Info");
	gtk_window_set_default_size(GTK_WINDOW (window), 430, 250);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(window), "configure-event", G_CALLBACK(resize), NULL);

	workArea = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), workArea);
//-------------------------------------------------------------------------------------------------
	nb = gtk_notebook_new();
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(nb), GTK_POS_BOTTOM);
	gtk_notebook_set_scrollable(GTK_NOTEBOOK(nb), TRUE);
	gtk_notebook_popup_enable(GTK_NOTEBOOK(nb));
	gtk_fixed_put(GTK_FIXED(workArea), GTK_WIDGET(nb), 0, 0);
	task();
//-------------------------------------------------------------------------------------------------	
	css = gtk_css_provider_new();
	gtk_css_provider_load_from_path(css, "style.css", NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(css), 									GTK_STYLE_PROVIDER_PRIORITY_USER);
//-------------------------------------------------------------------------------------------------
	gtk_widget_show_all(window);
	gtk_main();	
	return 0;
}
