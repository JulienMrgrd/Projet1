#include"pageJeu.h"
#include "utils.h"

static GtkWidget *fenetre = NULL;
static GtkBuilder *builder = NULL;

static GtkWidget *text_view;
static GtkTextBuffer *buffer;
static GtkWidget *scrolled_window;
static GtkTextIter iter;

static GtkLabel *labPhase = NULL;
static GtkLabel *labMeilleurProp = NULL;
static GtkLabel *labRecapPartie = NULL;
static GtkButton *buttonSoumission = NULL;
static GError *error = NULL;
static int isButtonXclicked=1;
static int isClosed = 0;
static GtkWidget *pTable;
static GtkLabel *pLabel[17][17];
static char* murLabel[17][17];
static pthread_t temps;
static char* name;
static int phaseReflexion=0; // Pase actuelle
static int phaseResolution=0;
static int phaseEnchere=0;
static int xR,xB,xJ,xV,xC; // coord x des robots et cibles
static int yR,yB,yJ,yV,yC; // coord y des robots cibles
static char cC[2];  // couleur cible
static int myCoup;
static int numChrono;

int proposition(GtkWidget * p_wid, gpointer p_data){
	GtkEntry* entry = (GtkEntry*)p_data;
	char* coups = gtk_entry_get_text(entry);

	if(strcmp(coups, "")){

		char messageEnvoye[70];

		if(phaseReflexion==1 && isdigit(*coups)){
			myCoup=atoi(coups);
			sprintf(messageEnvoye, "TROUVE/%s/%s/\n",name,coups);
			sendToServer(messageEnvoye);
		} else if(phaseEnchere==1 && isdigit(*coups)){
			myCoup=atoi(coups);
			sprintf(messageEnvoye, "ENCHERE/%s/%s/\n",name,coups);
			sendToServer(messageEnvoye);
		} else if(phaseResolution==1){
			sprintf(messageEnvoye, "SOLUTION/%s/%s/\n",name,coups);
			sendToServer(messageEnvoye);
		}

		gtk_entry_set_text(entry, "");
	}
	printf("end proposition\n");
	return 0;
}

/**
 * Modifie le label "meilleurProposition" pour la phase d'enchere, pour permettre au joueur
 * de connaitre la meilleure proposition
 * @param username : nom du joueur ayant proposé la meilleure solution
 * @param nbCoups : nombre de coups de la meilleure solution
 */
void meilleureProposition(char* username, char* nbCoups){
	char message[60];
	if(nbCoups==NULL) sprintf(message, "Meilleure proposition : %s = %d", username, myCoup);
	else sprintf(message, "Meilleure proposition : %s = %d", username, atoi(nbCoups));

	gdk_threads_enter();
	gtk_label_set_text(labMeilleurProp, message);
	gdk_threads_leave();
}


void auTourDe(char *user){
	char message[60];
	if(strlen(user)!=0) {
		sprintf(message, "Au tour de : %s",user);
		gdk_threads_enter();
		gtk_label_set_text(labMeilleurProp, message);
		gdk_threads_leave();
	}else{
		sprintf(message, "");
	}
	gdk_threads_enter();
	gtk_label_set_text(labMeilleurProp, message);
	gdk_threads_leave();
}


/**
 * Modification de la phase du jeu, modifie le labels "phase", le texte du bouton "soumission"
 * et déclenche le chrono correspondant à la nouvelle phase
 * @param phase : nom de la nouvelle phase où nous passons
 */
void setPhase(char *phase){

	if(strstr(phase,"ENCHERE")){
		gdk_threads_enter();
		gtk_label_set_text(labPhase, "Phase d'enchere");
		gtk_button_set_label(buttonSoumission, "Votre enchère");
		gdk_threads_leave();
		phaseReflexion=0;
		phaseResolution=0;
		phaseEnchere=1;
		numChrono++;
		pthread_create(&temps, NULL, threadChrono, 40);
	}
	if(strstr(phase,"RESOLUTION")){
		gdk_threads_enter();
		gtk_label_set_text(labPhase, "Phase de resolution");
		gtk_button_set_label(buttonSoumission, "Votre solution");
		gdk_threads_leave();
		phaseEnchere=0;
		phaseReflexion=0;
		phaseResolution=1;
		numChrono++;
		pthread_create(&temps, NULL, threadChrono, 60);
	}
	if(strstr(phase,"REFLEXION")){
		gdk_threads_enter();
		gtk_label_set_text(labPhase, "Phase de reflexion");
		gtk_button_set_label(buttonSoumission, "Votre nombre de coups");
		gdk_threads_leave();
		phaseResolution=0;
		phaseReflexion=1;
		phaseEnchere=0;
		numChrono++;
		pthread_create(&temps, NULL, threadChrono, 300);
		auTourDe("");
	}
}

int chat(GtkWidget * p_wid, gpointer p_data){

	GtkEntry* entry = (GtkEntry*)p_data;

	char messageEnvoye[200];
	char message[140];

	sprintf(message, "%s",  gtk_entry_get_text(entry));
	if(strlen(message)!=0){
		sprintf(messageEnvoye, "CHAT/%s/%s\n",name,message);
		sendToServer(messageEnvoye);
	}
	gtk_entry_set_text(entry, "");

	return 0;

}

/**
 * Ajout des murs composant le plateau dans un tableau permettant le stockage de ceux-ci
 * @param x : coordonnées en abscisse du mur a ajouter
 * @param y : coordonnées en ordonnée du mur à ajouter
 * @param mur : type de mur a ajouter dans le tableau (H: Haut, B: Bas, G: Gauche, D: Droit)
 */
void addMurTableau(int x, int y, char *mur){
	if (strstr(murLabel[x][y], mur) == NULL) {
		if(!strcmp(mur,"H")){
			strcat(murLabel[x][y+1],"B");
		}else {
			strcat(murLabel[x][y],mur);
		}
	}
}

/**
 * Ajout des murs de base composant tous les plateaux de Rasende-Roboter©
 * mur entourant le plateau, plus le carré central
 */
void addMurTableauBase(){
	int i;
	int j;
	for(i=0; i<16;i++){
		for(j=15;j>=0;j--){
			if(i==0){
				addMurTableau(i,j,"G");
			} else if(i==15){
				addMurTableau(i,j,"D");
			}
			if(j==0){
				addMurTableau(i,j,"B");
			} else if(j==15){
				addMurTableau(i,j,"H");
			}
		}
	}
	addMurTableau(7,8,"G");
	addMurTableau(7,9,"B");
	addMurTableau(8,7,"D");
	addMurTableau(8,7,"B");
	addMurTableau(8,9,"B");
	addMurTableau(8,8,"D");
}

/**
 * Retourne la valeur hexadecimal de la couleur de la cible a ajouter au plateau
 * @param couleur : Couleur de la cible (cR: cible Rouge, cB: cible Bleu, cJ: cible Jaune, cV: cible Verte)
 */
char* getCouleurCible(char* couleur){
	char coul[8];
	if(!strcmp(couleur, "cR")){
		sprintf(coul,"FF0000");
	}else if(!strcmp(couleur, "cA")){
		sprintf(coul, "0000FF");
	}else if(!strcmp(couleur, "cJ")){
		sprintf(coul, "EFD807");
	}else { // cV
		sprintf(coul, "00FF00");
	}
	return coul;
}

/**
 * Affichage des 4 robots et de la cible sur le plateau
 */
void displayRobot(){
	char couleur[8] = "";
	int x, y;
	int i;
	for(i=0; i<5; i++){
		if(i==0){
			x=xR;
			y=yR;
			sprintf(couleur, "FF0000");
		}else if(i==1){
			x=xB;
			y=yB;
			sprintf(couleur, "0000FF");
		}else if(i==2){
			x=xJ;
			y=yJ;
			sprintf(couleur, "EFD807");
		}else if(i==3){
			x=xV;
			y=yV;
			sprintf(couleur, "00FF00");
		} else {
			x=xC;
			y=yC;
			sprintf(couleur, getCouleurCible(cC));
		}
		printf("couleur %s\n", couleur);

		char res[100] = "";
		if(strstr(murLabel[x][y], "G")){
			if((x!=0)&&(y!=16)&& (strstr(murLabel[x-1][y],"D") &&  ((strstr(murLabel[x-1][y+1],"B")) || (strstr(murLabel[x-1][y],"B"))))
					&& 	!strstr(murLabel[x][y], "B") && !strstr(murLabel[x][y+1], "B") ){
				strcat(res, " ");
			}else{
				strcat(res, "|");
			}
		}else {
			strcat(res, "  ");
		}

		char lettre = 'R';
		if(i==4) lettre='C';

		if(strstr(murLabel[x][y], "B")){
			sprintf(res, "%s<span foreground=\"#%s\" face=\"Sans\"><u><small>%c</small></u></span>",res, couleur,lettre);
		}else{
			sprintf(res, "%s<span foreground=\"#%s\" face=\"Sans\"><small>%c</small></span>",res,couleur,lettre);
		}

		if(strstr(murLabel[x][y], "D")){
			if( (x!=15) && (y!=16) && (strstr(murLabel[x+1][y],"G")
					&&  ((strstr(murLabel[x+1][y+1],"B")) || (strstr(murLabel[x+1][y],"B"))) )){
				strcat(res, " ");
			}else{
				strcat(res, "|");
			}
		}else {
			strcat(res, "  ");
		}

		gdk_threads_enter();
		gtk_label_set_use_markup(pLabel[x][y], TRUE);
		gtk_label_set_markup(pLabel[x][y], res);
		gdk_threads_leave();
	}
	printf("Fin for display robot\n");
}

/**
 * Affichage du plateau avec tous ces murs, mais sans les robots
 */
void displayPlateau(){
	printf("start display\n");
	int i;
	int j;
	for(i=0; i<16; i++){
		for(j=16; j>=0; j--){
			displayCase(i, j);
		}
	}
	printf("après for display\n");

	char* xSouligne = "|<span face=\"Sans\"><small><small><small>    </small></small></small></span>";
	char* secSouligne = "|<span face=\"Sans\"><u><small><small>sec</small></small></u></span>";

	gdk_threads_enter();
	gtk_label_set_use_markup(pLabel[7][7], TRUE);
	gtk_label_set_markup(pLabel[7][7], secSouligne);
	gtk_label_set_use_markup(pLabel[7][8], TRUE);
	gtk_label_set_markup(pLabel[7][8], xSouligne);
	gdk_threads_leave();

	printf("fin display\n");
}

/**
 * Suppression des donnees de tous les robots et effaçage des robots du plateau
 */
void resetRobot(){

	printf("Entre dans reset Robot\n");
	int x, y;
	int i;

	for(i=0;i<5;i++){
		if(i==0){
			x=xR;
			y=yR;
		}else if(i==1){
			x=xB;
			y=yB;
		}else if(i==2){
			x=xJ;
			y=yJ;
		}else if(i==3){
			x=xV;
			y=yV;
		}else if(i==4){
			x=xC;
			y=yC;
		}
		displayCase(x, y);

	}
	sprintf(cC, "");
}

/**
 * Affichage d'une case en fonction de ces coordonnées
 * @param x : coordonnée en abscisse de la case (0 = tout à gauche)
 * @param y : coordonnée en ordonnée de la case (0 = tout en bas)
 */
void displayCase(int x, int y){
	char res[10] = "";
	if(strstr(murLabel[x][y], "G")){
		if((x!=0)&&(y!=16)&& (strstr(murLabel[x-1][y],"D") &&  ((strstr(murLabel[x-1][y+1],"B")) || (strstr(murLabel[x-1][y],"B"))))
				&& 	!strstr(murLabel[x][y], "B") && !strstr(murLabel[x][y+1], "B") ){
			strcat(res, " ");
		}else{
			strcat(res, "|");
		}
	}else if(y==0 || y==16){
		strcat(res, ".");
	}else{
		strcat(res, " ");
	}

	if(strstr(murLabel[x][y], "B")) strcat(res, "_");
	else strcat(res, "  ");


	if(strstr(murLabel[x][y], "D")){
		if( (x!=15) && (y!=16) && (strstr(murLabel[x+1][y],"G")
				&&  ((strstr(murLabel[x+1][y+1],"B")) || (strstr(murLabel[x+1][y],"B"))) )){
			strcat(res, " ");
		} else {
			strcat(res, "|");
		}
	}else if(y==0 || y==16){
		strcat(res, ".");
	}else{
		strcat(res, " ");
	}


	gdk_threads_enter();
	gtk_label_set_text( pLabel[x][y], res);
	gdk_threads_leave();
}

/**
 * Ajout de tous les murs composants le plateau dans le tableau composant tous les murs
 * @param plateau : plateau de la session courante (sous la forme "(X,Y,{H,B,G,D})(X,Y,{H,B,G,D})...")
 */
void addMurTableauPlateau(char* plateau){

	char** splitParentOuvr = splitWithChar(plateau, '(');
	char** splitParentFerm;
	char** splitVirgule;
	char* tmp;
	int j=0;
	int i=0;
	for (i=0; (splitParentOuvr[i] != NULL) ; i++) {
		if(strlen(splitParentOuvr[i])!=0){
			splitParentFerm = splitWithChar(splitParentOuvr[i], ')');
			for(j=0; (splitParentFerm[j] != NULL) ; j++){
				splitVirgule = splitWithChar(splitParentFerm[j], ',');
				addMurTableau(atoi(splitVirgule[0]),atoi(splitVirgule[1]),(splitVirgule[2]));
			}
		}
	}
}

/**
 * Stockage des données des 4 robots et de la cible en fonction de l'enigme
 * @param enigme : enigme du tour positionnant les robots et la cible (sous la forme "(xR,yR,xB,yB,xJ,yJ,xV,yV,xC,yC,C)" x et y étant des entiers)
 */
void addRobotCible(char* enigme){
	printf("On entre dans addRobotCible \n");
	printf("enigme avant addRobot == %s\n",enigme);

	char *p = enigme; // suppr 1ier et dernier char (les parentheses)
	p++;
	p[strlen(p)-1] = 0;

	printf("enigme après addRobot == %s\n",p);

	char** splitVirgule = splitWithChar(p, ',');

	xR=atoi(splitVirgule[0]);
	yR=atoi(splitVirgule[1]);
	xB=atoi(splitVirgule[2]);
	yB=atoi(splitVirgule[3]);
	xJ=atoi(splitVirgule[4]);
	yJ=atoi(splitVirgule[5]);
	xV=atoi(splitVirgule[6]);
	yV=atoi(splitVirgule[7]);
	xC=atoi(splitVirgule[8]);
	yC=atoi(splitVirgule[9]);

	strcat(cC, "c");
	if(strstr(splitVirgule[10],"B")){
		strcat(cC, "A");
	}else{
		strcat(cC, splitVirgule[10]);
	}

}

/**
 * Declenchement et affichage du chrono au centre du plateau en seconde
 * @param chrono : Durée voulu pour chaque tour
 */
void threadChrono(int chrono){
	char temps[100];
	gdk_threads_enter();
	gtk_label_set_use_markup(pLabel[7][7], TRUE);
	gdk_threads_leave();
	int phaseResol=phaseResolution;
	int phaseEnch=phaseEnchere;
	int phaseReflex=phaseReflexion;
	int nbChrono=numChrono;

	while((chrono>=0) && (phaseReflex==phaseReflexion) && (phaseEnch==phaseEnchere)
			&& (phaseResol==phaseResolution) && (nbChrono==numChrono)){ //On teste si on change pas de phase
		if(chrono < 100){
			if(chrono < 10){
				sprintf(temps, "|  <span face=\"Sans\"><small>%d</small></span>", chrono);
			}else{
				sprintf(temps, "| <span face=\"Sans\"><small><small>%d</small></small></span>", chrono);
			}
		}else{
			sprintf(temps, "|<span face=\"Sans\"><small><small><small>%d</small></small></small></span>", chrono);
		}
		gdk_threads_enter();
		gtk_label_set_markup(pLabel[7][8], temps);
		gdk_threads_leave();
		sleep(1);
		chrono--;
	}
}

/**
 * Modification du label "recapPartie" pour afficher la liste des joueurs et leur score
 * @param bilan : bilan de la partie (sous la forme "n(user1, X)(user2, Y)...")
 */
void affichageBilan(char *bilan){
	printf("on entre dans affichage bilan\n");
	printf("bilan ==== %s\n",bilan);

	char** splitParentOuvr = splitWithChar(bilan, '(');
	char** splitParentFerm;
	char** splitVirgule;
	char bilanAffich[300] = "";
	char tmp[50] = "";
	if(!strcmp(splitParentOuvr[0],"1")){
		sprintf(tmp,"Vous etes au tour %s\n",splitParentOuvr[0]);
	}else {
		sprintf(tmp,"Vous etes au tours %s\n",splitParentOuvr[0]);
	}
	strcat(bilanAffich,tmp);
	int j;
	int i;
	for (i=1; (splitParentOuvr[i] != NULL) ; i++) {
		if(strlen(splitParentOuvr[i])!=0){
			splitParentFerm = splitWithChar(splitParentOuvr[i], ')');
			for(j=0; (splitParentFerm[j] != NULL) ; j++){
				splitVirgule = splitWithChar(splitParentFerm[j], ',');
				sprintf(tmp,"%s : %s points\n",splitVirgule[0],splitVirgule[1]);
				strcat(bilanAffich,tmp);
			}
		}
	}
	gdk_threads_enter();
	gtk_label_set_text(labRecapPartie, bilanAffich);
	gdk_threads_leave();
}


/**
 * Modification du label "recapPartie" pour afficher le noms et le score du vainqueur d'une session
 * @param bilan : bilan de la partie (sous la forme "n(user1, X)(user2, Y)...")
 */
void displayVainqueur(char *bilan){
	printf("on entre dans affichage bilan\n");
	printf("bilan ==== %s\n",bilan);

	char** splitParentOuvr = splitWithChar(bilan, '(');
	char** splitParentFerm;
	char** splitVirgule;
	int highScore=0;
	char bestPlayer[16]="";
	int j;
	int i;
	for (i=1; (splitParentOuvr[i] != NULL) ; i++) {
		if(strlen(splitParentOuvr[i])!=0){
			splitParentFerm = splitWithChar(splitParentOuvr[i], ')');
			for(j=0; (splitParentFerm[j] != NULL) ; j++){
				splitVirgule = splitWithChar(splitParentFerm[j], ',');
				if(atoi(splitVirgule[1])>=highScore){
					highScore=atoi(splitVirgule[1]);
					sprintf(bestPlayer,"%s",splitVirgule[0]);
				}
			}
		}
	}
	char msgVainqueur[100] = "";
	sprintf(msgVainqueur,"Nous avons un vainqueur !!\n %s avec un score de %d\n",bestPlayer,highScore);

	gdk_threads_enter();
	gtk_label_set_text(labRecapPartie, msgVainqueur);
	gdk_threads_leave();
}


/**
 * Lancement de la phase de refelexion
 * @param enigme : enigme du tour positionnant les robots et la cible (sous la forme "(xR,yR,xB,yB,xJ,yJ,xV,yV,xC,yC,C)" x et y étant des entiers)
 * @param bilan : bilan de la partie (sous la forme "n(user1, X)(user2, Y)...")
 */
void startReflexion(char* enigme, char* bilan){
	resetRobot();

	char* bilan_copy = strdup(bilan);
	char* enigme_copy = strdup(enigme);

	affichageBilan(bilan);
	addRobotCible(enigme);
	setPhase("REFLEXION");
	displayRobot();
}

/**
 * Lancement de l'affichage de la page de jeu, initialisation des labels et des tableau et récuperation des pointeurs de chaque éléments de l'affcihage à manipuler dans le futur
 *  @param plateau : plateau de la session courante (sous la forme "(X,Y,{H,B,G,D})(X,Y,{H,B,G,D})...")
 *  @param pseudo : pseudo de l'utilisateur jouant
 */
int startPageJeu(char* plateau, char* pseudo){

	int i=0;
	int j=0;
	int x=0;
	int y=0;
	numChrono = 0;
	printf("on entre dans StartPageJEu\n");
	for(x=0;x<17;x++){
		for(y=0;y<17;y++){
			murLabel[x][y] = malloc(4 * sizeof(char));
			murLabel[x][y] = strdup("");
			pLabel[x][y] = gtk_label_new("   ");
		}
	}

	printf("apresStartPageJEu\n");
	name = pseudo;
	printf("apres name\n");
	if( !g_thread_supported()) g_thread_init( NULL );
	gdk_threads_init();

	addMurTableauBase();
	addMurTableauPlateau(plateau);
	displayPlateau();
	printf("after display !\n");

	isClosed = 0;


	printf("Before init \n");
	gdk_threads_enter();

	builder = gtk_builder_new();
	gchar* filename =  g_build_filename ("glade_files/pageJeu.glade", NULL);

	gtk_builder_add_from_file (builder, filename, &error);
	g_free (filename);
	if (error){
		gint code = error->code;
		g_printerr("%s\n", error->message);
		g_error_free (error);
		return code;
	}
	fenetre = GTK_WIDGET(gtk_builder_get_object (builder, "window1"));
	labPhase = GTK_WIDGET(gtk_builder_get_object (builder, "phase"));
	labRecapPartie = GTK_WIDGET(gtk_builder_get_object (builder, "recapPartie"));
	labMeilleurProp = GTK_WIDGET(gtk_builder_get_object (builder, "meilleurProposition"));
	buttonSoumission = GTK_BUTTON(gtk_builder_get_object(builder, "soumission"));

	buffer = gtk_text_buffer_new(NULL);
	text_view = GTK_WIDGET(gtk_builder_get_object (builder, "textview"));
	gtk_text_view_set_buffer (text_view, buffer);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_view), GTK_WRAP_WORD);
	scrolled_window = GTK_WIDGET(gtk_builder_get_object (builder, "scrolledwindow"));
	gtk_text_buffer_get_start_iter(buffer, &iter);

	gtk_misc_set_alignment(labMeilleurProp,0,0.5);
	gtk_misc_set_alignment(labRecapPartie,0.2,0.1);

	pTable=gtk_table_new(48,20,FALSE);
	gtk_container_add(GTK_CONTAINER(gtk_builder_get_object (builder, "vpaned13")), GTK_WIDGET(pTable));

	for(i=0;i<16;i++){
		for(j=16;j>=0;j--){
			gtk_table_attach(GTK_TABLE(pTable), pLabel[i][j],
					i*3,i*3+2, 16-j, 16-j+1,
					GTK_SHRINK ,  GTK_SHRINK,
					0, 0);
		}
	}


	g_signal_connect (gtk_builder_get_object (builder, "soumission"), "clicked", G_CALLBACK (proposition),(gpointer)(gtk_builder_get_object(builder, "proposition")));
	g_signal_connect (gtk_builder_get_object (builder, "bouttonChat"), "clicked", G_CALLBACK (chat),(gpointer)(gtk_builder_get_object(builder, "messageChat")));

	gtk_widget_show_all (fenetre);

	gtk_main();
	gdk_threads_leave();

	isClosed = 1;
	return isButtonXclicked;
}

/**
 * Destruction de la fenetre de jeu
 */
void destroyPageJeu(){
	printf("Destruction de la fenetre de jeu\n");
	isButtonXclicked=0;
	gdk_threads_enter();
	gtk_widget_destroy(fenetre);
	gdk_threads_leave();
	printf("Destroy de jeu reussi\n");
}


/**
 * Ajout et affichage du message reçu par le serveur dans le label "messageServeur"
 * @param message : message a ajouter au label pour qu'il soit afficher
 */
void addMessageServerPageJeu(char* message){
	if(isClosed==1) return; // la fenêtre a été fermée
	printf("Ajouter message server\n");

	printf("Avant ajouter message jeu\n");
	char newMessage[strlen(message)+1];
	if(buffer!=NULL && strlen(buffer)>0){
		strcpy(newMessage, "\n");
		strcat(newMessage, message);
	} else {
		strcpy(newMessage, message);
	}

	gtk_text_buffer_insert(buffer, &iter, newMessage, -1);
	gtk_text_buffer_get_end_iter(buffer,&iter);
	printf("Après ajouter message jeu\n");
}
