#include "common.h"
#include "WindowMain.h"

int main(int argc,char *argv[]){
	
	gtk_init(&argc, &argv);
	
	main_window_init();

	g_object_unref(G_OBJECT(builder));

	gtk_widget_show(window_main);
	gtk_main();
	return 0;
}