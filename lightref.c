
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>

static gboolean continue_timer = FALSE;
static gboolean start_timer = FALSE;
static gboolean gamma_change_active = FALSE;
static char monitor_name[128] = {0}; // Initialize as an empty string

static int sec_expired = 0;

int blueFilterDurationSec = 5;
static int screenTimeAllowed = 5;                                       // edit according to your preference

static int total_time = 0;
static int blueFilterTime = 0;

static int userChoice = 0; // Variable to store user's choice

static void on_option_selected(GtkWidget *window1, gpointer data) {
    userChoice = GPOINTER_TO_INT(data);
    printf("%d\n",userChoice);
    gtk_window_close(GTK_WINDOW(window1));
    // gtk_widget_hide(widget);
}

static void show_popup_menu() {
    GtkWidget *window1;
    GtkWidget *box;
    GtkWidget *button_notification;
    GtkWidget *button_timer;
    GtkWidget *button_notification_timer;

    gtk_init(NULL, NULL);

    window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window1), "Choose an Option");
    g_signal_connect(G_OBJECT(window1), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);

    button_notification = gtk_button_new_with_label("Notification");
    g_signal_connect(G_OBJECT(button_notification), "clicked", G_CALLBACK(on_option_selected), GINT_TO_POINTER(1));
    gtk_container_add(GTK_CONTAINER(box), button_notification);

    button_timer = gtk_button_new_with_label("Blue Ray Filter");
    g_signal_connect(G_OBJECT(button_timer), "clicked", G_CALLBACK(on_option_selected), GINT_TO_POINTER(2));
    gtk_container_add(GTK_CONTAINER(box), button_timer);

    button_notification_timer = gtk_button_new_with_label("Notification and Blue Ray Filter");
    g_signal_connect(G_OBJECT(button_notification_timer), "clicked", G_CALLBACK(on_option_selected), GINT_TO_POINTER(3));
    gtk_container_add(GTK_CONTAINER(box), button_notification_timer);

    gtk_container_add(GTK_CONTAINER(window1), box);

    gtk_widget_show_all(window1);

    gtk_main();
}



static void getMonitorName() {
    const char *command = "xrandr --listmonitors";
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error running the command");
        return;
    }

    char buffer[1024];
    int line_count = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        line_count++;
        if (line_count == 2) {
            char *last_space = strrchr(buffer, ' ');
            if (last_space != NULL) {
                strncpy(monitor_name, last_space + 1, sizeof(monitor_name) - 1);
                // monitor_name[sizeof(monitor_name) - 1] = '\0'; // Ensure null-termination
                printf("Monitor Name: %s\n", monitor_name);
            } else {
                printf("No monitor name found in the output.\n");
            }
            break;
        }
    }
    pclose(fp);
}

static void change_gamma() {
    char sysCall1[128];
    const char *str1 = "xrandr --output ";
    const char *str2 = " --gamma 1.0:1.0:0.5";
    getMonitorName();

    // char sysCall1[100]; // Assuming a maximum length for the result
    int i, j, k;

    // Copy str1 into result
    for (i = 0; str1[i] != '\0'; i++) {
        sysCall1[i] = str1[i];
    }

    // Concatenate str2 to result
    for (j = 0; monitor_name[j] != '\0'; j++) {
        sysCall1[i + j] = monitor_name[j];
    }

    for (k = 0; str2[k] != '\0'; k++) {
        sysCall1[j+i+k-1] = str2[k];
    }
    sysCall1[i + j+ k-1] = '\0';

    // snprintf(sysCall1, sizeof(sysCall1), "xrandr --output %s --gamma 1.0:1.0:0.5", monitor_name);
    printf("%s\n", sysCall1);

    
    if(userChoice == 1 || userChoice == 3){
        system("zenity --info --text=\"You have been using the screen continuously for the past 20 minutes. We recommend you to take a 20 second break now. \n"
           "Please relax your eyes and focus on some distant object before resuming your work again!\n\n Blue light filter activated for the next 180 seconds.\"");
    }
    if(userChoice == 2 || userChoice == 3){
        system(sysCall1);
    }

}

static void reset_gamma() {
    char sysCall1[128];
    const char *str1 = "xrandr --output ";
    const char *str2 = " --gamma 1.0:1.0:1.0";
    getMonitorName();

    // char sysCall1[100]; // Assuming a maximum length for the result
    int i, j, k;

    // Copy str1 into result
    for (i = 0; str1[i] != '\0'; i++) {
        sysCall1[i] = str1[i];
    }

    // Concatenate str2 to result
    for (j = 0; monitor_name[j] != '\0'; j++) {
        sysCall1[i + j] = monitor_name[j];
    }

    for (k = 0; str2[k] != '\0'; k++) {
        sysCall1[j+i+k-1] = str2[k];
    }
    sysCall1[i + j+ k-1] = '\0';

    // snprintf(sysCall1, sizeof(sysCall1), "xrandr --output %s --gamma 1.0:1.0:0.5", monitor_name);
    printf("%s\n", sysCall1);

    system(sysCall1);

}



static void
_quit_cb (GtkWidget *button, gpointer data)
{
    (void)button; (void)data; // Avoid compiler warnings
    if (gamma_change_active) {
        reset_gamma();
    }
    gtk_main_quit();

    char quitNotifCommand[1000];
    sprintf(quitNotifCommand,"zenity --info --text=\"Thank you for using visionCare. "
                             "Your health is our utmost priority.\n\n"
                             "Your total screen time was %d minutes and %d seconds.\n"
                             "Time spent on screen with Blue Filter : %d seconds.\"",total_time/60,total_time%60,blueFilterTime);
    system(quitNotifCommand);

}

static gboolean
_label_update(gpointer data)
{
    GtkLabel *label = (GtkLabel *)data;
    char buf[256];
    memset(&buf, 0x0, 256);

    int remTime = gamma_change_active ? blueFilterDurationSec - sec_expired : screenTimeAllowed - sec_expired;

    int minutes = remTime / 60;
    int seconds = remTime % 60;

    if(gamma_change_active){
        snprintf(buf, 255, "Blue filter active for: %d min %d sec", minutes, seconds);
        
    }
    else{
        snprintf(buf, 255, "Next screen break in: %d min %d sec", minutes, seconds);
    }

    ++sec_expired;
    ++total_time;
    if(gamma_change_active) blueFilterTime++;

    if (sec_expired >=screenTimeAllowed  && gamma_change_active==FALSE) {                                       // 20 minutes
        printf("Changed\n");
        if (!gamma_change_active) {
            change_gamma();
            gamma_change_active = TRUE;
            sec_expired = 0;

        }
    } else if (gamma_change_active && sec_expired >= blueFilterDurationSec) {                                       // 20 seconds
        reset_gamma();
        gamma_change_active = FALSE;
        sec_expired = 0;
    }

    gtk_label_set_label(label, buf);
    return continue_timer;
}

static void
_start_timer (GtkWidget *button, gpointer data)
{
    (void)button;//Avoid compiler warnings/
    GtkWidget *label = data;
    if(!start_timer)
    {
        g_timeout_add_seconds(1, _label_update, label);
        start_timer = TRUE;
        continue_timer = TRUE;
    }
}

static void
_pause_resume_timer (GtkWidget *button, gpointer data)
{
    (void)button;//Avoid compiler warnings/
    if(start_timer)
    {
        GtkWidget *label = data;
        continue_timer = !continue_timer;
        if(continue_timer)
        {
            g_timeout_add_seconds(1, _label_update, label);
        }
        else
        {
            //Decrementing because timer will be hit one more time before expiring/
            sec_expired--;
        }
    }
}

static void
_reset_timer (GtkWidget *button, gpointer data)
{
    (void)button; (void)data;//Avoid compiler warnings/
    //Setting to -1 instead of 0, because timer will be triggered once more before expiring/
    sec_expired = -1;
    continue_timer = FALSE;
    start_timer = FALSE;
}

int main(void)
{
    show_popup_menu();
    // initialChoice();
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *start_button;
    GtkWidget *pause_resume_button;
    GtkWidget *reset_button;
    GtkWidget *quit_button;
    GtkWidget *label;
    // show_popup_menu();
    getMonitorName();

    if (userChoice == 1){
        blueFilterDurationSec = 0;
    }


    // printf("%s", monitor_name);


    gtk_init(NULL, NULL);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect (G_OBJECT (window), "destroy",
                      G_CALLBACK (gtk_main_quit),
                      NULL);
    vbox = gtk_box_new (FALSE, 2);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    label = gtk_label_new("Loading...");

    start_button = gtk_button_new_with_label("Start");
    g_signal_connect(G_OBJECT(start_button), "clicked", G_CALLBACK(_start_timer), label);

    pause_resume_button = gtk_button_new_with_label("Pause/Resume");
    g_signal_connect(G_OBJECT(pause_resume_button), "clicked", G_CALLBACK(_pause_resume_timer), label);

    reset_button = gtk_button_new_with_label("Reset");
    g_signal_connect(G_OBJECT(reset_button), "clicked", G_CALLBACK(_reset_timer), label);

    quit_button = gtk_button_new_with_label("Quit");
    g_signal_connect(G_OBJECT(quit_button), "clicked", G_CALLBACK(_quit_cb), NULL);

    gtk_box_pack_start (GTK_BOX(vbox), label, 0, 0, 0);
    gtk_box_pack_start (GTK_BOX(vbox), start_button, 0, 0, 0);
    gtk_box_pack_start (GTK_BOX(vbox), pause_resume_button, 0, 0, 0);
    gtk_box_pack_start (GTK_BOX(vbox), reset_button, 0, 0, 0);
    gtk_box_pack_start (GTK_BOX (vbox), quit_button, 0, 0, 0);

    gtk_widget_show_all(window);

    g_timeout_add_seconds(1, _label_update, label);
    continue_timer = TRUE;
    start_timer = TRUE;

    gtk_main();
    return 0;
}