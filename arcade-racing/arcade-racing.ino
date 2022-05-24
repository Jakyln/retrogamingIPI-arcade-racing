
#include <Gamebuino-Meta.h>

// Eggs
const int NBR_D_OEUFS = 10;
int oeufsX[NBR_D_OEUFS] = {};
int oeufsY[NBR_D_OEUFS] = {};

const int OEUF_LARGEUR = 2;
const int OEUF_HAUTEUR = 3;

// Carots
const int NBR_MAX_DE_CAROTTES = 8;
int nbrDeCarottes = 0;
int carottesX[NBR_MAX_DE_CAROTTES] = {};
int carottesY[NBR_MAX_DE_CAROTTES] = {};

const int CAROTTE_LARGEUR = 3;
const int CAROTTE_HAUTEUR = 5;

// Panier
int panierX = 20;  // Le panier peut bouger horizontalement, donc ce n'est pas une constante
const int PANIER_Y = gb.display.height() - 4;  // Constante car le panier est fixe selon l'axe Y
int panierLargeur = 5;  // Pas constant car la taille dépend de la nourriture restante
const int PANIER_HAUTEUR = 3;  // Constante

int score = 0;
int timer = 0;
int highscore = 0;
int nourriture;  // Plus de nourriture est mieux. 0 nourriture = game over

void setup() {
  gb.begin();
  gb.display.println("Pour commencer, appuyer sur le bouton A");
  if (gb.buttons.pressed(BUTTON_A)) {
    gb.display.println("Pour commencer, appuyer sur le bouton A");
    // Placer les oeufs aléatoirement au dessus de l'écran
    for (int i = 0; i < NBR_D_OEUFS; i += 1) {
      oeufsX[i] = random(0, gb.display.width());
      oeufsY[i] = random(-40, -OEUF_HAUTEUR);  // Au dessus de l'écran
    }
    nourriture = 20;
  }
}

void loop() {
  while (!gb.update());
  entrees();
  miseAJour();
  affichage();
}

void entrees() {
  if (gb.buttons.repeat(BUTTON_LEFT, 0) && panierX > 0) {
    panierX -= 2;
  }
  else if (gb.buttons.repeat(BUTTON_RIGHT, 0) && panierX < gb.display.width()) {
    panierX += 2;
  }
}

void miseAJour() {
  timer++;
  if (nourriture <= 0) {  // Somme nous mort de faim??
    nbrDeCarottes = 0;  // Reset les carottes
    score = 0;  // Recommencer le jeu
    timer = 0;
    nourriture = 20;  // Reset nourriture
    for (int i = 0; i < NBR_D_OEUFS; i += 1) {
      oeufsX[i] = random(0, gb.display.width());
      oeufsY[i] = random(-40, -OEUF_HAUTEUR);  // Au dessus de l'écran
    }
  }

  // Eggs
  for (int i = 0; i < NBR_D_OEUFS; i += 1) {
    oeufsY[i] += 3;

    // Collisions
    if (gb.collide.rectRect(panierX - panierLargeur / 2, PANIER_Y, panierLargeur, PANIER_HAUTEUR, oeufsX[i], oeufsY[i], OEUF_LARGEUR, OEUF_HAUTEUR)) {
      score += 1;
      if (score > highscore) {
        highscore = score;
      }
      nourriture = nourriture - 2;  // MMmmmmm des oeufs
      // Ajouter des carrotes. Sans dépasser le plafond
      nbrDeCarottes = min(score / 5, NBR_MAX_DE_CAROTTES);

      // Reset l'oeuf
      oeufsX[i] = random(0, gb.display.width());
      oeufsY[i] = random(-40, -OEUF_HAUTEUR);  // Au dessus de l'écran
    }

    // Verifier que la carotte ne soit pas sortie de l'écran
    if (oeufsY[i] >= gb.display.height()) {
      // Reset l'oeuf
      oeufsX[i] = random(0, gb.display.width());
      oeufsY[i] = random(-40, -OEUF_HAUTEUR);  // Au dessus de l'écran
    }
  }

  // Carottes
  for (int i = 0; i < nbrDeCarottes; i += 1) {
    carottesY[i] += 2;

    // Collisions avec le joueur
    if (gb.collide.rectRect(panierX - panierLargeur / 2, PANIER_Y, panierLargeur, PANIER_HAUTEUR, carottesX[i], carottesY[i], CAROTTE_LARGEUR, CAROTTE_HAUTEUR)) {

      nourriture -= 4;  // Allergie => moins de nourriture :(

      // Reset la carotte
      carottesX[i] = random(0, gb.display.width());
      carottesY[i] = random(-20, -CAROTTE_HAUTEUR);  // Au dessus de l'écran
    }

    // Verifier que la carotte ne soit pas sortie de l'écran
    if (carottesY[i] >= gb.display.height()) {
      // Reset la carotte
      carottesX[i] = random(0, gb.display.width());
      carottesY[i] = random(-20, CAROTTE_HAUTEUR);  // Au dessus de l'écran
    }
  }

}

void affichage() {
  gb.display.clear();

  // Eggs
  gb.display.setColor(BROWN);
  for (int i = 0; i < NBR_D_OEUFS; i += 1) {
    gb.display.fillRect(oeufsX[i], oeufsY[i], OEUF_LARGEUR, OEUF_HAUTEUR);
  }

  // Carrots
  gb.display.setColor(ORANGE);
  for (int i = 0; i < nbrDeCarottes; i += 1) {
    gb.display.fillRect(carottesX[i], carottesY[i], CAROTTE_LARGEUR, CAROTTE_HAUTEUR);
  }

  // Player
  gb.display.setColor(WHITE);
  gb.display.fillRect(panierX - panierLargeur / 2, PANIER_Y, panierLargeur, PANIER_HAUTEUR);

  // Score
  gb.display.setColor(WHITE);
  gb.display.println("Points :");
  gb.display.print(timer);
  gb.display.setColor(RED);
  gb.display.setCursor(gb.display.width() - 8, 0);
  gb.display.print(nourriture);

}
