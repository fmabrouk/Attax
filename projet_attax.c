#include <string.h>
#include <MLV/MLV_all.h>
#define TAILLE_PLATEAU 7
#define TAILLE_MAX_NOM 20
#define TAILLE_CASE 100
#define TAILLE_MARGE 300

typedef struct {
    char nom[TAILLE_MAX_NOM];
    char symbole;
    int score;
} Joueur;

typedef struct {
    char plateau[TAILLE_PLATEAU + 2][TAILLE_PLATEAU + 2];
    Joueur* joueurs[2];
} Plateau;

void init_Plateau(Plateau* p){
	int i,j;
	for(i = 0; i < TAILLE_PLATEAU+2;i++){
		for(j = 0; j < TAILLE_PLATEAU+2;j++){
			if(i == 0 || i == TAILLE_PLATEAU + 1 || j == 0 || j == TAILLE_PLATEAU + 1){ // Les bords du plateau
				p->plateau[i][j] = '*';
			}
			else{ // Les cases vides du plateau
				p->plateau[i][j] = '.';
			}
		}
	}
    // Les pions initiales du jeu
	p->plateau[1][1] = 'x';
	p->plateau[1][TAILLE_PLATEAU] = 'o';
	p->plateau[TAILLE_PLATEAU][1] = 'o';
	p->plateau[TAILLE_PLATEAU][TAILLE_PLATEAU] = 'x';
}

void afficherPlateau(Plateau p) {
    // Fonction qui affiche le plateau dans le terminal
    for (int i=0; i<TAILLE_PLATEAU+2; i++) {
        for (int j=0; j<TAILLE_PLATEAU+2; j++) {
            printf("%2c", p.plateau[i][j]);
        }printf("\n");
    }
}

void affich_grille(Plateau p, Joueur j1, Joueur j2){
    // Fonction qui affiche la grille dans une fenêtre graphique
    int y;
    // on dessine les cases
    for(y=0; y<=TAILLE_PLATEAU; y++){
        MLV_draw_line (0, y*TAILLE_CASE, TAILLE_PLATEAU*TAILLE_CASE, y*TAILLE_CASE, MLV_COLOR_BLUE1);
        MLV_draw_line (y*TAILLE_CASE, 0, y*TAILLE_CASE, TAILLE_PLATEAU*TAILLE_CASE, MLV_COLOR_BLUE1);
    }
    // on traduit les symboles des joueurs dans le plateau en cercles colorés sur la grille
    for (int i=1; i<TAILLE_PLATEAU+1; i++) {
        for (int j=1; j<TAILLE_PLATEAU+1; j++) {
            if (p.plateau[i][j] == j1.symbole)
                MLV_draw_filled_circle( i*TAILLE_CASE - TAILLE_CASE/2, j*TAILLE_CASE - TAILLE_CASE/2, TAILLE_CASE/2, MLV_COLOR_WHITE);
            if (p.plateau[i][j] == j2.symbole)
                MLV_draw_filled_circle( i*TAILLE_CASE - TAILLE_CASE/2, j*TAILLE_CASE - TAILLE_CASE/2, TAILLE_CASE/2, MLV_COLOR_RED);
        }
    }
    // Le nom du jeu
    MLV_draw_adapted_text_box(TAILLE_PLATEAU*TAILLE_CASE+TAILLE_MARGE*1/4, TAILLE_MARGE*1/4, "ATTAXX", TAILLE_CASE/2, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_RED, MLV_TEXT_CENTER);
}

void init_joueur(Joueur* j1, Joueur* j2){
    // Fonction initialisant deux joueurs
	j1->score = 2;
	j2->score = 2;
	j1->symbole = 'x';
	j2->symbole = 'o';
	printf("Quel est le nom du premier joueur (symbol x) : ");
	scanf("%s",j1->nom);
	printf("Quel est le nom du deuxième joueur (symbol o) : ");
	scanf("%s",j2->nom);
}

void jouer(int *x, int *y, Joueur j){
    // Fonction qui récupère les coordonnées tapés pas le joueur sur le terminal
    printf("%s (%c), veuillez saisir les coordonnees ou jouer (entre 1 et %d) : ",j.nom, j.symbole, TAILLE_PLATEAU);
    scanf(" %d", x);
    scanf(" %d", y);
}

void pixel_vers_case(int x, int y, int *i, int *j) {
    // Fonction qui récupère les coordonnées i et j d'une case à partir des valeurs x et y en pixels
    *i = x / TAILLE_CASE +1;
    *j = y / TAILLE_CASE +1;
}

int cases_voisines(int x, int y, char symbole_adv, Plateau p){
    /* Vérifie s'il y au moins un pion adverse aux coordonnées adjacentes*/
    int i, j;
    for(i = x-1; i <= x+1; i++){
        for(j = y-1; j <= y+1; j++){
            if (p.plateau[i][j] == symbole_adv){
                return 1;
            };
        }
    }
    return 0;
}

int coup_valide(int x, int y, char symbole_adv, Plateau p){
    /* Vérifie si la case est bien dans le plateau, si elle a une case voisine contenant un pion adverse
    et si elle est vide */
    if((x < 1 || y < 1 || x > TAILLE_PLATEAU || y > TAILLE_PLATEAU)
    || (cases_voisines(x, y, symbole_adv, p) == 0)
    || (p.plateau[x][y] != '.')){
        return 0;
    }
    
    return 1;
}


void modif_plateau(Plateau *p, Joueur *j1, Joueur *j2, int x, int y){
    // Modifie le plateau en plaçant le pion aux coordonnées et en changeant le symbole des cases adjacentes
    (*p).plateau[x][y] = (*j1).symbole;
    (*j1).score +=1;
    for(int i = x-1; i <= x+1; i++){
        for(int j = y-1; j <= y+1; j++){
            if ((*p).plateau[i][j] == (*j2).symbole){
                (*p).plateau[i][j] = (*j1).symbole;
                (*j1).score +=1;
                (*j2).score -=1;
            }
        }
    }
}

int gagner(Joueur j1, Joueur j2, Plateau p, int cmpt_tours){
    if (j2.score == 0)
        return 1; // premier joueur en paramètre gagne
    if (j1.score == 0)
        return 2; // second joueur en paramètre gagne
    if (cmpt_tours >= (TAILLE_PLATEAU * TAILLE_PLATEAU) - 4){
        if (j1.score > j2.score) return 1;
        if (j2.score > j1.score) return 2;
        else return 3; // égalité
    }
    else return 0; // Ce n'est pas encore la fin du jeu
}

void partie_ascii(){
    Plateau p;
    Joueur j1, j2;
    init_Plateau(&p);
    init_joueur(&j1, &j2);
    afficherPlateau(p);
    p.joueurs[0] = &j1; p.joueurs[1] = &j2;
    int cmpt_tours = 0;
    int x, y;
    while (gagner(j1, j2, p, cmpt_tours) == 0){
        if (cmpt_tours % 2 == 0){ // le tour du premier joueur
            jouer(&x, &y, j1);
            while (coup_valide(x, y, j2.symbole, p) == 0){
                jouer(&x, &y, j1);
            }
            modif_plateau(&p, &j1, &j2, x, y);
        }
        else { // le tour du second joueur
            jouer(&x, &y, j2);
            while (coup_valide(x, y, j1.symbole, p) == 0){
                jouer(&x, &y, j2);
            }
            modif_plateau(&p, &j2, &j1, x, y);
        }
        cmpt_tours ++;
        afficherPlateau(p);
		printf("Score actuel: %s(x) %d - %s(o) %d\n\n",j1.nom, j1.score, j2.nom, j2.score);
	}
	if (gagner(j1, j2, p, cmpt_tours) == 1){
		printf("Bravo %s, vous avez gagné %d à %d.\n\n",j1.nom, j1.score, j2.score);
	}
	if (gagner(j1, j2, p, cmpt_tours) == 2){
		printf("Bravo %s, vous avez gagné %d à %d.\n\n", j2.nom, j2.score, j1.score);
	}
	if (gagner(j1, j2, p, cmpt_tours) == 3){
		printf("Egalité %d à %d.\n\n", j2.score, j1.score);
	}
}
void partie_graphique(){
    Plateau p;
    Joueur j1, j2;
    init_Plateau(&p);
    init_joueur(&j1, &j2);
    p.joueurs[0] = &j1; p.joueurs[1] = &j2;
    MLV_create_window( "Version graphique", "",TAILLE_PLATEAU*TAILLE_CASE + TAILLE_MARGE, TAILLE_PLATEAU*TAILLE_CASE);
    int cmpt_tours = 0;
    int x, y, a, b;
    while (gagner(j1, j2, p, cmpt_tours) == 0){
        if (cmpt_tours % 2 == 0){ // le tour du premier joueur
            affich_grille(p, j1, j2);
            MLV_draw_text(TAILLE_PLATEAU*TAILLE_CASE+TAILLE_MARGE*1/4, TAILLE_PLATEAU*TAILLE_CASE/2, "Le joueur actif est %s", MLV_COLOR_WHITE, j1.nom);
            MLV_update_window();
            MLV_wait_mouse(&x, &y);
            pixel_vers_case(x, y, &a, &b);
            while (coup_valide (a, b, j2.symbole, p) == 0){
                MLV_wait_mouse(&x, &y);
                pixel_vers_case(x, y, &a, &b);
            }
            modif_plateau(&p, &j1, &j2, a, b);
            affich_grille(p, j1, j2);
            MLV_update_window();
            MLV_clear_window(MLV_COLOR_BLACK);
        }
        else { // le tour du second joueur
            affich_grille(p, j1, j2);
            MLV_draw_text(TAILLE_PLATEAU*TAILLE_CASE+TAILLE_MARGE*1/4, TAILLE_PLATEAU*TAILLE_CASE/2, "Le joueur actif est %s", MLV_COLOR_WHITE, j2.nom);
            MLV_update_window();
            MLV_wait_mouse(&x, &y);
            pixel_vers_case(x, y, &a, &b);
            while (coup_valide (a, b, j1.symbole, p) == 0){
                MLV_wait_mouse(&x, &y);
                pixel_vers_case(x, y, &a, &b);
            }
            modif_plateau(&p, &j2, &j1, a, b);
            affich_grille(p, j1, j2);  
            MLV_update_window();
            MLV_clear_window(MLV_COLOR_BLACK);
        }
        cmpt_tours ++;
	}
	if (gagner(j1, j2, p, cmpt_tours) == 1){
		printf("Bravo %s, vous avez gagné %d à %d.\n\n",j1.nom, j1.score, j2.score);
	}
	if (gagner(j1, j2, p, cmpt_tours) == 2){
		printf("Bravo %s, vous avez gagné %d à %d.\n\n", j2.nom, j2.score, j1.score);
	}
	if (gagner(j1, j2, p, cmpt_tours) == 3){
		printf("Egalité %d à %d.\n\n", j2.score, j1.score);
	}
    MLV_free_window();
}

int main(int argc, char *argv[]){
    if ((argc < 2) || (argv[1][0] != '-')){
        printf("Il faut choisir le mode d'affichage après l'éxécutable : \n-a si vous souhaitez un affichage en ASCII\n-g si  vous souhaitez un affichage graphique\nPuis choisir entre jouer avec un humain (en ajoutant h) ou un ordinateur (pas encore disponible)\n");
        return 0;
    }
    int a, g, h, o, i = 0;
    while (argv[1][i] != '\0') {
        switch (argv[1][i]) {
            case 'a':
                a = 1;
                g = 0;
                break;
            case 'g':
                a = 0;
                g = 1;
                break;
            case 'h':
                h = 1;
                o = 0;
                break;
            case 'o':
                h = 0;
                o = 1;
                break;
        }
        i++;
    }
    if ((a==1) && (h==1)){
        partie_ascii();
    }
    if ((g==1) && (h==1)){
        partie_graphique();
    }
    // Il manque l'option de jouer contre l'ordinateur.
	return 0;
}