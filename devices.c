/* Devices */
#include "disk.h"

GtkWidget *topgrid;	/* Grid at the top */
GtkWidget *updt;	/* Update button */

/* Tabs */
GtkWidget *devtabs;
GtkWidget *tabusb;
GtkWidget *tabpci;
GtkWidget *tabacpi;

GtkWidget *pcibox;
GtkWidget *usbbox;
GtkWidget *acpibox;


GtkTreeStore *devts;

/* Device class, e.g. wireless */
struct {
GtkTreeIter network;
GtkTreeIter multimedia;
GtkTreeIter display;
GtkTreeIter bridge;
GtkTreeIter comms;
GtkTreeIter serialbus;
GtkTreeIter storage;
GtkTreeIter peripheral;

GtkTreeIter unclassified;
GtkTreeIter memory;
GtkTreeIter inputdevice;
GtkTreeIter docking;
GtkTreeIter processor;
GtkTreeIter wireless;
GtkTreeIter satellite;
GtkTreeIter intelligent;
GtkTreeIter encrypt;
GtkTreeIter signalproc;
GtkTreeIter accelerator;
/* and more ... */
} pciclass;

//GtkTreeIter parent;
//GtkTreeIter	child;	/* Device */

void dev_cleanup() {

	/* 
	 * Destroy the top level box and
	 * the gtknotebook.
	 */
	if(GTK_IS_WIDGET(devbox))
		gtk_widget_destroy(devbox);

	if(GTK_IS_WIDGET (devtabs))
		gtk_widget_destroy(devtabs);

	if(GTK_IS_WIDGET (tabpci))
		gtk_widget_destroy(tabpci);

	if(GTK_IS_WIDGET (tabusb))
		gtk_widget_destroy(tabusb);

	if(GTK_IS_WIDGET (tabacpi))
		gtk_widget_destroy(tabacpi);

	if(GTK_IS_WIDGET (pcibox))
		gtk_widget_destroy(pcibox);

	if(GTK_IS_WIDGET (usbbox))
		gtk_widget_destroy(usbbox);

	if(GTK_IS_WIDGET (acpibox))
		gtk_widget_destroy(acpibox);

devbox = NULL;
devtabs = NULL;
tabusb = NULL;
tabpci = NULL;
tabacpi = NULL;

pcibox = NULL;
usbbox = NULL;
acpibox = NULL;

}

GtkWidget *add_scrolled(GtkWidget *box) {
	/* Scrolled window. */
	GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC); 
	gtk_box_pack_start(GTK_BOX(box), scrolled_window, FALSE, TRUE, 0);
	gtk_scrolled_window_set_propagate_natural_height (GTK_SCROLLED_WINDOW(scrolled_window), TRUE);
	gtk_scrolled_window_set_propagate_natural_width (GTK_SCROLLED_WINDOW(scrolled_window), TRUE);

	return scrolled_window;
}

gboolean perms() {
	if(!root()) {
		if(pw_needed() == TRUE) {
			/* We need the passwort! */
			return FALSE;
		}
	}
	return TRUE;
}

GtkWidget *new_treeview(GtkWidget *box, char array[10][20], int members) {

	GtkWidget *view = gtk_tree_view_new();
	
	gtk_container_add(GTK_CONTAINER(box), view);	
	GtkCellRenderer *cell = gtk_cell_renderer_text_new();

	g_object_set(cell, "font", opt.fontsize, NULL);
	//g_object_set(cell,"editable", TRUE, NULL);
	//gtk_tree_view_set_enable_search(view, TRUE);
	g_object_set(view, "enable-grid-lines", GTK_TREE_VIEW_GRID_LINES_BOTH, NULL);
	
	int cnt = 0;
	while((cnt < members) && (strlen(array[cnt]) > 0)) {

		/* First column */
		char *col_title = array[cnt];

		GtkTreeViewColumn *col = gtk_tree_view_column_new();
		gtk_tree_view_column_set_title(col, col_title);
		gtk_tree_view_column_set_clickable(col, TRUE);
		gtk_tree_view_column_set_reorderable(col, TRUE);
		gtk_tree_view_column_set_resizable(col, TRUE);
  		gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

		gtk_tree_view_column_pack_start(col, cell, TRUE);
		gtk_tree_view_column_add_attribute(col, cell, "text", (cnt));

		cnt++;
		}

	/* We always use strings. */
	devts = gtk_tree_store_new(members, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, 	G_TYPE_STRING, \
		G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

	gtk_tree_view_set_model(GTK_TREE_VIEW(view), GTK_TREE_MODEL(devts));
	g_object_unref(devts); /* destroy model automatically with view. */
  	gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)),
                             GTK_SELECTION_SINGLE);

	return view;
}

void fill_pci_tv(GtkWidget *tv) {

	/*
	 * Add default device classes.
	 * to the treeview.
	 */
			gtk_tree_store_append(devts, &pciclass.display, NULL);
			gtk_tree_store_set(devts, &pciclass.display, 0, "Display", -1);

			gtk_tree_store_append(devts, &pciclass.network, NULL);
			gtk_tree_store_set(devts, &pciclass.network, 0, "Network", -1);

			gtk_tree_store_append(devts, &pciclass.multimedia, NULL);
			gtk_tree_store_set(devts, &pciclass.multimedia, 0, "Multimedia", -1);

			gtk_tree_store_append(devts, &pciclass.storage, NULL);
			gtk_tree_store_set(devts, &pciclass.storage, 0, "Mass Storage", -1);
	
			gtk_tree_store_append(devts, &pciclass.peripheral, NULL);
			gtk_tree_store_set(devts, &pciclass.peripheral, 0, "Peripherals", -1);

			gtk_tree_store_append(devts, &pciclass.inputdevice, NULL);
			gtk_tree_store_set(devts, &pciclass.inputdevice, 0, "Inputdevice", -1);

			gtk_tree_store_append(devts, &pciclass.bridge, NULL);
			gtk_tree_store_set(devts, &pciclass.bridge, 0, "Bridge", -1);

			gtk_tree_store_append(devts, &pciclass.comms, NULL);
			gtk_tree_store_set(devts, &pciclass.comms, 0, "Simple Comms", -1);

			gtk_tree_store_append(devts, &pciclass.serialbus, NULL);
			gtk_tree_store_set(devts, &pciclass.serialbus, 0, "Serial Bus", -1);

			gtk_tree_store_append(devts, &pciclass.unclassified, NULL);
			gtk_tree_store_set(devts, &pciclass.unclassified, 0, "Other", -1);

/*
 * Execute pciconf -l
 * Add each line to row+column
 */


	char line[MAXLINE];
	int size = MAXLINE;
	int i;
	int col = 0;
	int last;
	GtkTreeIter	iter;
	GtkTreeIter parent;

	memset(line, 0, MAXLINE);

	FILE *fp = popen("pciconf -lv", "r");
	if(fp == NULL) {
		printf("could not execute pciconf -lv\n");	
		return;
	}

	/* Get a line */
	while( fgets(line, sizeof line, fp) ) {

		if(line[0] != ' ') { /* device info, first line */

			
			/* Driver name */
			while(line[i] != '@')
				i++;
			line[i] = '\0';

			/* PCI slot */
			i++;
			int slot = i;
			while((line[i] != ' ') && (line[i] != '\t'))
				i++;
			line[i] = '\0';

			/* class */
			while(line[i] != '=')
				i++;
			i++;
			int class = i;
			while(line[i] != ' ')
				i++;
			line[i] = '\0';
			
			/* card */
			while(line[i] != '=')
				i++;
			i++;
			int card = i;
			while(line[i] != ' ')
				i++;
			line[i] = '\0';

			/* chip */
			while(line[i] != '=')
				i++;
			i++;
			int chip = i;
			while(line[i] != ' ')
				i++;
			line[i] = '\0';

			if(strncmp(&line[class], "0x06", 4) == 0)
				parent = pciclass.bridge;
			else if(strncmp(&line[class], "0x03", 4) == 0)
				parent = pciclass.display;
			else if(strncmp(&line[class], "0x0c", 4) == 0)
				parent = pciclass.serialbus;
			else if(strncmp(&line[class], "0x04", 4) == 0)
				parent = pciclass.multimedia;
			else if(strncmp(&line[class], "0x01", 4) == 0)
				parent = pciclass.storage;
			else if(strncmp(&line[class], "0x07", 4) == 0)
				parent = pciclass.comms;
			else if(strncmp(&line[class], "0x08", 4) == 0)
				parent = pciclass.peripheral;
			else if(strncmp(&line[class], "0x02", 4) == 0)
				parent = pciclass.network;
			else if(strncmp(&line[class], "0x09", 4) == 0)
				parent = pciclass.inputdevice;
			else
				parent = pciclass.unclassified;


			gtk_tree_store_append(devts, &iter, &parent);
			gtk_tree_store_set(devts, &iter, 0, &line[0], 3, &line[card], 4, &line[chip], 5, &line[slot], 6, &line[class], -1);
		}

		else { /* -v options */

			/* Skip space characters. */
			while(line[i] == ' ')
				i++;

			/* Determine the correct column. */
			if(strncmp(&line[i], "vendor", 6) == 0)
				col = 2;
			else if	(strncmp(&line[i], "device", 6) == 0)
				col = 1;
			else if	(strncmp(&line[i], "class", 5) == 0)
				col = 7;
			else if	(strncmp(&line[i], "subclass", 8) == 0)
				col = 8;

			/* Read the variable. */
			while((line[i] != '=') && (i < MAXLINE))
				i++;
			i++;
			
			/* Zero terminate? */
			last = i;
			while((line[last] != '\n') && (last < MAXLINE))
				last++;
			line[last] = '\0';

			/* Write the variable to the treestore. */
			gtk_tree_store_set(devts, &iter, col, &line[i], -1);
		}
		memset(line, 0, MAXLINE);
		i = 0;
	}
	pclose(fp);

	/* Be nice to viewers. */
	gtk_tree_view_expand_all(GTK_TREE_VIEW(tv));
	gtk_tree_view_set_enable_search (GTK_TREE_VIEW(tv), TRUE);	

}

void devpci() {

	/* Cleanup */
	if(GTK_IS_WIDGET(pcibox))
		gtk_widget_destroy(pcibox);

	/* A container. */
	pcibox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	gtk_container_add (GTK_CONTAINER (tabpci), pcibox);

	/* A scrolled window */
	GtkWidget *w = add_scrolled(pcibox);

	/* A Treeview */
	GtkWidget *pci_tv = new_treeview(w, pcicol, PCICOL);
	fill_pci_tv(pci_tv);

	gtk_widget_show_all(pcibox);
}

void fill_usb_tv(GtkWidget *tv) {

		
	/* 
	 * Check for permissions.
	 * If we don't have them, ask for pw. Then come back.
	 * 
	 * If we have them, execute usbconfig.
	 * Output goes in the treeview. 
	 */
	if((!root()) && (pw_needed() == TRUE)) {
		char *cmd = malloc(20);
		strcpy(cmd, "whoami");
		window_pw(cmd);
		return;
	}

	char line[MAXLINE];
	int size = MAXLINE;
	int i = 0;
	int col = 0;
	GtkTreeIter	iter;

	memset(line, 0, MAXLINE);

	FILE *fp = popen("sudo usbconfig", "r");
	if(fp == NULL) {
		printf("devices.c: Could not execute sudo usbconfig\n");	
		return;
	}

	/* Get a line */
	while( fgets(line, sizeof line, fp) ) {

		gtk_tree_store_append(devts, &iter, NULL);	

		/* Name */
		while(line[i] != ':')
			i++;
		line[i] = '\0';

		/* Description */
		while(line[i] != '<')
			i++;
		i++;
		int desc = i;
		
		while(line[i] != '>')
			i++;
		line[i] = '\0';

		/* Bus */
		i = i + 5;
		int bus = i;
		while(line[i] != ',')
			i++;
		line[i] = '\0';

		/* cfg */
		while(line[i] != '=')
			i++;
		i++;
		int cfg = i;
		
		while(line[i] != ' ')
			i++;
		line[i] = '\0';
		
		/* Mode */
		while(line[i] != '=')
			i++;
		i++;
		int m = i;
		while(line[i] != ' ')
			i++;
		line[i] = '\0';

		/* Speed */
		while(line[i] != '=')
			i++;
		i++;
		int speed = i;
		while(line[i] != ')')
			i++;
		i++;
		line[i] = '\0';

		/* Power */
		while(line[i] != '=')
			i++;
		i++;
		int power = i;
		while(line[i] != ' ')
			i++;
		line[i] = '\0';

		/* Power2 */
		while(line[i] != '(')
			i++;
		i++;
		int power2 = i;
		while(line[i] != ')')
			i++;
		line[i] = '\0';

		gtk_tree_store_set(devts, &iter, 0, &line[0], 1, &line[desc], 2, &line[bus], 3, &line[cfg], 4,\
		 &line[m], 5, &line[speed], 6, &line[power], 7, &line[power2], -1);

		memset(line, 0, MAXLINE);
		i=0;
	}
}

void devusb() {

	/* Cleanup */
	if(GTK_IS_WIDGET(usbbox))
		gtk_widget_destroy(usbbox);

	/* A container. */
	usbbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	gtk_container_add (GTK_CONTAINER (tabusb), usbbox);
	
	GtkWidget *w = add_scrolled(usbbox);
	GtkWidget *usb_tv = new_treeview(w, usbcol, USBCOL);
	fill_usb_tv(usb_tv);

	gtk_widget_show_all(usbbox);
}

void devacpi() {

	/* Cleanup */
	if(GTK_IS_WIDGET(acpibox))
		gtk_widget_destroy(acpibox);

	/* A container. */
	acpibox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	gtk_container_add (GTK_CONTAINER (tabacpi), acpibox);
	
	/* A scrolled window */
	GtkWidget *w = add_scrolled(acpibox);
	gtk_container_add (GTK_CONTAINER (w), gtk_label_new("Not implemented yet!"));
	
	gtk_widget_show_all(acpibox);
}

void on_dev_changed(GtkMenuItem *item, gpointer p) {

	gint n = gtk_notebook_get_current_page(GTK_NOTEBOOK (devtabs));
	const gchar *tab = gtk_notebook_get_tab_label_text(GTK_NOTEBOOK (devtabs), gtk_notebook_get_nth_page(GTK_NOTEBOOK (devtabs), n));
	
	if(tab == NULL)
		return;
	else if(strcmp(tab, "PCI") == 0)
		devpci();
	else if(strcmp(tab, "USB") == 0)
		devusb();
	else if(strcmp(tab, "ACPI") == 0)
		devacpi();
}

void devices_add_tabs() {
	
	devtabs = gtk_notebook_new();
//	gtk_notebook_set_tab_pos(task_tabs, GTK_POS_LEFT);
	gtk_notebook_set_show_border(GTK_NOTEBOOK (devtabs), FALSE);
	gtk_container_add (GTK_CONTAINER (devbox), devtabs);

	/* Two tabs. */
	tabusb = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	tabpci = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	tabacpi = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

	gtk_notebook_append_page(GTK_NOTEBOOK (devtabs), tabpci, gtk_label_new("PCI"));
	gtk_notebook_append_page(GTK_NOTEBOOK (devtabs), tabusb, gtk_label_new("USB"));
	gtk_notebook_append_page(GTK_NOTEBOOK (devtabs), tabacpi, gtk_label_new("ACPI"));

	g_signal_connect_after(G_OBJECT (devtabs), "switch-page", G_CALLBACK(on_dev_changed), devtabs);
}

void devices() {

	/* Cleanup */
	dev_cleanup();

	/* A top level container. */
	devbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	gtk_container_add (GTK_CONTAINER (tab8), devbox);

	/* Add tabs. */
	devices_add_tabs();

	gtk_widget_show_all(devbox);
}
