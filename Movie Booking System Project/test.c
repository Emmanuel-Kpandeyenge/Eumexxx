#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void menuScreen();
static void userRegister();
static void login();
static void mainScreen();
void currentMovie(char old[][40], int x);
static void upcomingMovie();
static void search();
static void admin();
static void adminMenu();
static void checkout();
static void debit();
static void credit();
static void paypal();
static void barcode();
static void newMainScreen();
static void barcodeReciept();
static void ticketSold();
static void manageShow();
static void view();
void add(char update[][40], int y);
void removee(char update[][40], int y);

void buyTickets(char str[]);
void writeReview(char str[]);
int rand_num;
int numT = 0;

struct login {
  char username[30];
  char password[20];
};

int main(void) {

  printf("Welcome to the Movie Booking System\n");
  menuScreen();
  return 0;
}

static void menuScreen() {
  printf("\t\tACTION MENU\n");
  printf("0- Register\n");
  printf("1- Login\n");
  printf("2- Admin\n");

  int numInput;
  printf("\nEnter action number:  ");
  scanf("%d", &numInput);

  if (numInput == 0) {
    userRegister();
  } else if (numInput == 1) {
    login();
  } else if (numInput == 2) {
    admin();
  } else {
    printf("\nNumber Entered Invalid. Try Again!\n");
    menuScreen();
  }
}
static void admin() {
  char password[] = "3365", p[20];
  int n = 1, y;

  printf("\nPassword:");
  scanf("%s", &p);
  fflush(stdout);

  y = strcmp(p, password);

  if (y == 0) {
    printf("\nSucessfully Logged In\n\n");
    adminMenu();
  } else {
    printf("\nWrong Password, try again", 1 - n);
    menuScreen();
    n++;
  }
}

static void adminMenu() {
  {
    system("clear");
    printf("\n\t\tAdmin Main Screen\n");
    printf("0- Tickets Sold\n");
    printf("1- Manage Showings\n");
    printf("2- Return to Menu Screen\n");

    int numInput;
    printf("\nEnter your choice :: ");
    scanf("%d", &numInput);

    switch (numInput) {
    case (0):
      ticketSold();
      break;
    case (1):
      manageShow();
      break;
    case (2):
      menuScreen();
      break;
    }
  }
}

static void ticketSold() {
  printf("%d", numT);
  int code;
  printf("\nPress 0 to Return to Admin Main Screen:  ");
  scanf("%d", &code);
  if (code == 0) {
    adminMenu();
  } else {
    adminMenu();
  }
}
static void login() {
  char username[30], password[20];
  FILE *log;
  log = fopen("login3.txt", "r");
  if (log == NULL) {
    fputs("Error at opening File!", stderr);
    exit(1);
  }

  struct login l;

  printf("Please Enter your login credentials below");
  printf("\nUsername:  ");
  scanf("%s", &username);
  printf("\nPassword: ");
  scanf("%s", &password);

  while (fread(&l, sizeof(l), 1, log)) {
    if (strcmp(username, l.username) == 0 &&
        strcmp(password, l.password) == 0) {
      mainScreen();
    } else {
      printf("\nIncorrect Login, Try again!\n");
      menuScreen();
    }
  } 

  fclose(log);
}

static void userRegister() {
  char name;
  char email;
  char home;
  int phoneNum;
  char pass;

  FILE *log;
  log = fopen("login3.txt", "w");
  if (log == NULL) {
    printf("ERROR! File cannot open!", stderr);
    exit(1);
  }

  struct login l;

  printf("Enter Name:\t");
  scanf("%s", &name);

  // fscanf(fp, "%s", &name);

  printf("\nEnter Email Address:\t");
  scanf("%s", &email);
  // fprintf(fp, "%s", &email);

  printf("\nEnter Home Address:\t");
  scanf("%s", &home);
  // fprintf(fp, "%s", &home);

  printf("\nEnter Phone Number:\t");
  scanf("%d", &phoneNum);
  // fprintf(fp, "%s", &phoneNum);

  printf("\nEnter Username:\t ");
  scanf("%s", l.username);

  printf("\nEnter Password:\t ");
  scanf("%s", l.password);

  fwrite(&l, sizeof(l), 1, log);
  fclose(log);
  printf("\nYou have successfully registered!\n");
  login();
}

static void mainScreen() {
  // need to be logined in or register to work
  system("clear");
  printf("\n\t\tMain Screen\n");
  printf("0- Browse Current Movie Catalog\n");
  printf("1- Browse Upcoming Movie Catalog\n");
  printf("2- Search Movies\n");
  printf("3- Admin\n");

  int numInput;
  printf("\nEnter your choice :: ");
  scanf("%d", &numInput);

  switch (numInput) {
  case (0): {
    int row = 5;
    char oldMovie[][40] = {"The Polar Express", "Bones and All",
                              "Strange World", "The Menu",
                              "Black Panther: Wakanda Forever"};
    currentMovie(oldMovie, row);
    break;
  }

  case (1):
    upcomingMovie();
    break;
  case (2):
    search();
    break;
  case (3):
    admin();
    break;
  }
}

void currentMovie(char old[][40], int x) {
  printf("\nCurrent Movies:: \n");
  // char oldMovie[5][20] = {"The Polar Express", "Bones and All", "Strange
  // World","The Menu", "Black Panther: Wakanda Forever"};

  for (int i = 0; i < x; i++) {
    printf("%d - %s\n", i, old[i]);
  }

  int movieNum;
  printf("\nWhich movie? \n(to go back to mainscreen enter 9) :\t");
  scanf("%d", &movieNum);

  switch (movieNum) {
  case (0): {
    printf("\n\t\t\t\t\tThe Polar Express (G)\n");
    printf(
        "Synopsis:: \nBased on the children's book by Chris Van Allsburg, this "
        "inspiring adventure tells the story of a young boy who doubts the "
        "existence of Santa Claus. Late one Christmas Eve night, he lies awake "
        "listening to hear the sounds of Santa's sleigh and reindeer. Instead, "
        "he hears the chugging of a steam locomotive and whistle, just outside "
        "his bedroom window. The train's conductor invites the boy on board "
        "for an extraordinary ride to the North Pole. It is a journey of "
        "self-discovery for the boy, showing him that the wonder of life never "
        "fades for those who choose to believe.\n");

    printf("\nCast: \nTom Hanks\nMichael Jeter\nChris Coppola\nJosh "
           "Hutcherson\nPeterScolari\n");

    printf("\n Runtime: 1h 40m\n");

    printf("\nReviews:\n");
    printf("\n\t 5/5 Stars! Very beautiful motion caption animation. The idea "
           "of the story is creative.\n");
    printf("\n\t 5/5 Stars! My all time favorite Christmas movie!\n");
    printf("\n\t1/5! The people just tried so hard to be realistic but the "
           "characters were dry and very odd.\n");

    char array[40] = {"The Polar Express"};
    printf("\n\n\tWhat next?\n");
    int nextPart = 0;
    printf("\n10- Buy Tickets\n11- Write a Review\n12- Go Back?\n");
    scanf("%d", &nextPart);
    if (nextPart == 10) {
      buyTickets(array);
    } else if (nextPart == 11) {
      writeReview(array);
    } else if (nextPart == 12) {
      currentMovie(old, x);
    } else {
      printf("Answer was not understood");
      mainScreen();
    }

    break;
  }

  case (1): {
    printf("\n\t\t\t\t\tBones and All (R)\n");
    printf("Synopsis: \nBONES AND ALL is a story of first love between Maren, "
           "a young woman learning how to survive on the margins of society, "
           "and Lee, an intense and disenfranchised drifter; a liberating road "
           "odyssy of two young people coming into their own.\n");
    printf("\nCast: \nTimothee Chalamet\nTaylor Russell\nMark Rylance\nAndre "
           "Holland\nJessia Harper\n");
    printf("\nRuntime: 2h 10m\n");
    printf("\nReviews: \n");
    printf(
        "\n\t4/5 stars! As unsettling as it is heart-racing. The denouement, "
        "too, is worth waiting for. Cannibalism has never looked prettier.\n");
    printf("\n\t1.5/5 stars! Filled with underdeveloped characters and a plot "
           "that goes nowhere, it reinforces the feeling I got from Luca "
           "Guadagninoâs Suspiria remake: this director doesnât understand "
           "horror but is really good at putting viewers to sleep.\n");
    printf("\n\t5/5 stars! Chalamet proved once and for all that he is the "
           "real deal.\n");

    char array[40] = {"Bones and All"};
    printf("\n\n\tWhat next?\n");
    int nextPart = 0;
    printf("\n10- Buy Tickets\n11- Write a Review\n12- Go Back?\n");
    scanf("%d", &nextPart);
    if (nextPart == 10) {
      buyTickets(array);
    } else if (nextPart == 11) {
      writeReview(array);
    } else if (nextPart == 12) {
      currentMovie(old, x);
    } else {
      printf("Answer was not understood");
      mainScreen();
    }

    break;
  }

  case (2): {
    printf("\n\t\t\t\t\tStrange World (PG)\n");
    printf("Synopsis: \nWalt Disney Animation Studios' original action-packed "
           "adventure Strange World introduces a legendary family of "
           "explorers, the Clades, as they attempt to navigate an uncharted, "
           "treacherous land alongside a motley crew.\n ");
    printf("\nCast: \nJake Gyllenhaal\n Jaboukie Young-White\n Gabrielle "
           "Union\n Dennis Quaid\n Lucy Liu\n");
    printf("\nRuntime: 1h 42m\n");
    printf("\nReviews: \n");
    printf("\n\t1/5 stars! This might have been an okay freeby on Disney, but "
           "dont't pay for this. What happened to decent writing?\n");
    printf("\n\t5/5 stars! I absolutely LOVED the movie.");

    char array[40] = {"Strange World"};
    printf("\n\n\tWhat next?\n");
    int nextPart = 0;
    printf("\n10- Buy Tickets\n11- Write a Review\n12- Go Back?\n");
    scanf("%d", &nextPart);
    if (nextPart == 10) {
      buyTickets(array);
    } else if (nextPart == 11) {
      writeReview(array);
    } else if (nextPart == 12) {
      currentMovie(old, x);
    } else {
      printf("Answer was not understood");
      mainScreen();
    }

    break;
  }

  case (3): {
    printf("\n\t\t\t\t\tThe Menu (R)\n");
    printf("Synopsis: \nA couple travels to a coastal island to eat at an "
           "exclusive restaurant where the chef has prepared a lavish menu, "
           "with some shocking surprises.\n");
    printf("\nCast: \nAnya Taylor-Joy\n Ralph Fiennes\n Nicholas Hoult\n John "
           "Leguizamo\n Janet McTeer\n");
    printf("\nRuntime: 1h 47m\n");
    printf("\nReviews: \n");
    printf("\n5/5 stars! So unpredictable, viciously funny thriller.\n");
    printf(
        "\n5/5 stars! Mark Mylod by far one of the best directors out their. "
        "The story, the acting, the picture overall was pure genius.");
    printf("\n5/5 stars! Best thriller this year. If you haven't seen it go "
           "now.\n");

    char array[40] = {"The Menu"};
    printf("\n\n\tWhat next?\n");
    int nextPart = 0;
    printf("\n10- Buy Tickets\n11- Write a Review\n12- Go Back?\n");
    scanf("%d", &nextPart);
    if (nextPart == 10) {
      buyTickets(array);
    } else if (nextPart == 11) {
      writeReview(array);
    } else if (nextPart == 12) {
      currentMovie(old, x);
    } else {
      printf("Answer was not understood");
      mainScreen();
    }

    break;
  }

  case (4): {
    printf("\n\t\t\t\t\tBlack Panther: Wakanda Forever (PG-13)\n");
    printf("Synopsis: \nQueen Ramonda, Shuri, M'Baku, Okoye and the Dora "
           "Milaje, fight to protect their nation from intervening world "
           "powers in the wake of King T'Challa's death. As the Wakandans "
           "strive to embrace their next chapter, the heroes must band "
           "together with the help of War Dog Nakia and Everett Ross and forge "
           "a new path for the kingdom of Wakanda.\n");
    printf("\nCast: \nLetitia Wright \nAngela Bassett\n Lupita Nyong'o\n Danai "
           "Gurira\n Winston Duke\n");
    printf("\nRuntime: 2h 41m \n");
    printf("\nReviews: \n");
    printf("\n\t4/5 stars! Following the first Black Panther was not an easy "
           "task, specially after loosing Chadwick. However they did an "
           "amazing job honoring his memory and picking up the story. \n");
    printf("\n\t4/5 stars! The performances are extermely personal and moving "
           "all around.\n");
    printf("\n\t5/5 stars! Namor by far best character. Wish he had more "
           "screen time tho.\n");

    char array[40] = {"Black Panther: Wakanda Forever"};
    printf("\n\n\tWhat next?\n");
    int nextPart = 0;
    printf("\n10- Buy Tickets\n11- Write a Review\n12- Go Back?\n");
    scanf("%d", &nextPart);
    if (nextPart == 10) {
      buyTickets(array);
    } else if (nextPart == 11) {
      writeReview(array);
    } else if (nextPart == 12) {
      currentMovie(old, x);
    } else {
      printf("Answer was not understood");
      mainScreen();
    }

    break;
  }
  case (5): {
    printf("\n\t\t\t\t\tViolent Night (R)\n");
    printf(
        "Synopsis: \n An elite team of mercenaries breaks into a family "
        "compound on Christmas Eve, taking everyone hostage inside. However, "
        "they aren't prepared for a surprise combatant: Santa Claus is on the "
        "grounds, and he's about to show why this Nick is no saint.\n");
    printf("\nCast:\nDavid Harbour\nJohn Leguizamo\nCam Gigandet\nAlex "
           "Hassell\nAlexis Louder \n");
    printf("\nRuntime: 1h 52m\n");
    printf("\n Reviews: \n");
    printf("\n\t4/5 starts! New twist on Christmas movie. Was gully of gory "
           "action with funny moments.\n");
    printf("\n\t5/5 stars! Put the kids to bed beofre watching this killer "
           "Santa movie. Definitely my new favorite Christmas movie.\n");
    printf("\n\t5/5 stars! I've never smiled so big at people getting crutally "
           "murdered.\n");

    char array[40] = {"Violent Night"};
    printf("\n\n\tWhat next?\n");
    int nextPart = 0;
    printf("\n10- Buy Tickets\n11- Write a Review\n12- Go Back?\n");
    scanf("%d", &nextPart);
    if (nextPart == 10) {
      buyTickets(array);
    } else if (nextPart == 11) {
      writeReview(array);
    } else if (nextPart == 12) {
      currentMovie(old, x);
    } else {
      printf("Answer was not understood");
      mainScreen();
    }
  }
  case (9): {
    mainScreen();
    break;
  }
  dafault : {
    printf("Input not recongized. Try again!");
    currentMovie(old, x);
  }
  }
}

static void upcomingMovie() {
  printf("\n Upcoming Movies:: \n");
  char movie[5][40] = {"Avatar: The Way of Water",
                       "Ant-Man and The Wasp: Quantumania", "Empire of Light",
                       "I Wanna Dance With Somebody", "Father Stu: Reborn"};

  for (int i = 0; i < 5; i++) {
    printf("%d - %s\n", i, movie[i]);
  }
  int movieNum;
  printf("\nWhich movie? \n(to go back to mainscreen enter 9) :\t");
  scanf("%d", &movieNum);

  switch (movieNum) {
  case (0): {
    printf("\n\t\t\t\t\tAvatar: The Way of Water (PG-13)\n");
    printf("Synopsis:: \nAvatar The Way of Water begins to tell the story of "
           "the Sully family, Jake, Neytiri and their kids, the trouble that "
           "follows \n");
    printf("\nCast: \nZoe Saldana\nSigourney Weaver\nSam Worthington\nStephen "
           "Lang\n");
    printf("\nRuntime: 2h 42m\n");
    printf("\nComing to theaters on December 16th, 2022!\n");
    break;
  }

  case (1): {
    printf("\n\t\t\tAnt-Man and The Wasp: Quantumania \n");
    printf("Synopsis:: \nSuper-Hero partners Scott Lang and Hope Van Dyne "
           "return to continue their adventures as Ant-Man and the Wasp. "
           "Together, with Hope's parents Hank Pym and Janet Van Dyne, the "
           "family finds themselves exploring the Quantum Realm. \n");
    printf("\nCast: \nPaul Rudd\nEvangeline Lilly\nMichael Douglas\nMichelle "
           "Pfeiffer\nKathryn Newton\n");
    printf("\nRuntime: to be determined\n");
    printf("\nComing to theaters on February 17th, 2023!\n");
    break;
  }
  case (2): {
    printf("\n\t\t\t\t\tEmpire of Light (R)\n");
    printf("Synopsis:: \nSet in an English seaside town in the early 1980s, "
           "EMPIRE OF LIGHT is a powerful and poignant story about human "
           "connection and the magic of cinema. \n");
    printf("\nCast: \nOlivia Colman\nMicheal Ward\nToby Jones\nColin Firth\n");
    printf("\nRuntime: 1h 53m \n");
    printf("\nComing to theathers on December 9th, 2022!\n");
    break;
  }
  case (3): {
    printf("\n\t\t\t\t\tI Wanna Dance With Somebody (G)\n");
    printf("Synopsis:: \nNaomi Ackie stars as Whitney Houston in the musical "
           "biopic, which is based on the epic life and music of the iconic "
           "singer. The film will take audiences on an emotional, energetic "
           "journey through Houston's career and music. \n");
    printf("\nCast: \nNaomi Ackie\nStanley Tucci\nAshyon Sanders\nTamara "
           "Tunie\nNafessa Williams\n");
    printf("\nRuntime: 2h 22m \n");
    printf("\nComing to theaters on December 21st, 2022!\n");
    break;
  }
  case (4): {
    printf("\n\t\t\t\t\tFather Stu: Reborn (PG-13)\n");
    printf("Synopsis:: \n Based on a true story, Father Stu: Reborn is a PG-13 "
           "take on the honest, funny, and ultimately uplifting drama about a "
           "lost soul who finds his purpose in a most unexpected place.\n");
    printf("\nCast: \nMark Wahlberg\nMel Gibson\nJacki Weaver\nTheresa Ruiz\n");
    printf("\nRuntime: 2h 4m \n");
    printf("\nComing to theaters on December 9th, 2022!\n");
    break;
  }
  case (9): {
    mainScreen();
    break;
  }
  dafault : {
    printf("Input not recongized. Try again!");
    upcomingMovie();
  }
  }
}

static void search() {
  printf("\nWelcome to Search Engine!");
  char word[40];
  printf("\nEnter the name of the movie you are looking for: \t");
  fgets(word, 40, stdin);
  // scanf("%[^\n]", word);

  printf("\nthe word entered was: %s", word);

  if (strcmp(word, "The Polar Express") == 0 ||
      strcmp(word, "the polar express") == 0) {
    printf("\nThere was a match!\n");
    printf("\n\t\t\t\t\tThe Polar Express (G)\n");
    printf(
        "Synopsis:: \nBased on the children's book by Chris Van Allsburg, this "
        "inspiring adventure tells the story of a young boy who doubts the "
        "existence of Santa Claus. Late one Christmas Eve night, he lies awake "
        "listening to hear the sounds of Santa's sleigh and reindeer. Instead, "
        "he hears the chugging of a steam locomotive and whistle, just outside "
        "his bedroom window. The train's conductor invites the boy on board "
        "for an extraordinary ride to the North Pole. It is a journey of "
        "self-discovery for the boy, showing him that the wonder of life never "
        "fades for those who choose to believe.\n");

    printf("\nCast: \nTom Hanks\nMichael Jeter\nChris Coppola\nJosh "
           "Hutcherson\nPeterScolari\n");

    printf("\n Runtime: 1h 40m\n");

    printf("\nReviews:\n");
    printf("\n\t 5/5 Stars! Very beautiful motion caption animation. The idea "
           "of the story is creative.\n");
    printf("\n\t 5/5 Stars! My all time favorite Christmas movie!\n");
    printf("\n\t1/5! The people just tried so hard to be realistic but the "
           "characters were dry and very odd.\n");

    char array[40] = {"The Polar Express"};
    printf("\n\n\tWhat next?\n");
    int nextPart = 0;
    printf("\n10- Buy Tickets\n11- Write a Review\n12- Go Back?\n");
    scanf("%d", &nextPart);
    if (nextPart == 10) {
      buyTickets(array);
    } else if (nextPart == 11) {
      writeReview(array);
    } else if (nextPart == 12) {
      mainScreen();
    } else {
      printf("Answer was not understood");
      mainScreen();
    }

  } else if (strcmp(word, "Bones and All") == 0 ||
             strcmp(word, "bones and all") == 0) {
    printf("\nThere was a match!\n");
    printf("\n\t\t\t\t\tBones and All (R)\n");
    printf("Synopsis: \nBONES AND ALL is a story of first love between Maren, "
           "a young woman learning how to survive on the margins of society, "
           "and Lee, an intense and disenfranchised drifter; a liberating road "
           "odyssy of two young people coming into their own.\n");
    printf("\nCast: \nTimothee Chalamet\nTaylor Russell\nMarkRylance\nAndre "
           "Holland\nJessia Harper\n");
    printf("\nRuntime: 2h 10m\n");
    printf("\nReviews: \n");
    printf(
        "\n\t4/5 stars! As unsettling as it is heart-racing. The denouement, "
        "too, is worth waiting for. Cannibalism has never looked prettier.\n");
    printf("\n\t1.5/5 stars! Filled with underdeveloped characters and a plot "
           "that goes nowhere, it reinforces the feeling I got from Luca "
           "Guadagninoâs Suspiria remake: this director doesnât understand "
           "horror but is really good at putting viewers to sleep.\n");
    printf("\n\t5/5 stars! Chalamet proved once and for all that he is the "
           "real deal.\n");

    char array[40] = {"Bones and All"};
    printf("\n\n\tWhat next?\n");
    int nextPart = 0;
    printf("\n10- Buy Tickets\n11- Write a Review\n12- Go Back?\n");
    scanf("%d", &nextPart);
    if (nextPart == 10) {
      buyTickets(array);
    } else if (nextPart == 11) {
      writeReview(array);
    } else if (nextPart == 12) {
      mainScreen();
    } else {
      printf("Answer was not understood");
      mainScreen();
    }

  } else if (strcmp(word, "Strange World") == 0 ||
             strcmp(word, "strange world") == 0) {
    printf("\nThere was a match!\n");
    printf("\n\t\t\t\t\tStrange World (PG)\n");
    printf("Synopsis: \nWalt Disney Animation Studios' original action-packed "
           "adventure Strange World introduces a legendary family of "
           "explorers, the Clades, as they attempt to navigate an uncharted, "
           "treacherous land alongside a motley crew.\n ");
    printf("\nCast: \nJake Gyllenhaal\n Jaboukie Young-White\nGabrielle "
           "Union\n Dennis Quaid\n Lucy Liu\n");
    printf("\nRuntime: 1h 42m\n");
    printf("\nReviews: \n");
    printf("\n\t1/5 stars! This might have been an okay freeby on Disney, but "
           "dont't pay for this. What happened to decent writing?\n");
    printf("\n\t5/5 stars! I absolutely LOVED the movie.");

    char array[40] = {"Strange World"};
    printf("\n\n\tWhat next?\n");
    int nextPart = 0;
    printf("\n10- Buy Tickets\n11- Write a Review\n12- Go Back?\n");
    scanf("%d", &nextPart);
    if (nextPart == 10) {
      buyTickets(array);
    } else if (nextPart == 11) {
      writeReview(array);
    } else if (nextPart == 12) {
      mainScreen();
    } else {
      printf("Answer was not understood");
      mainScreen();
    }

  } else if (strcmp(word, "The Menu") == 0 || strcmp(word, "the menu") == 0) {
    printf("\nThere was a match!\n");
    printf("\n\t\t\t\t\tThe Menu (R)\n");
    printf("Synopsis: \nA couple travels to a coastal island to eat at an "
           "exclusive restaurant where the chef has prepared a lavish menu, "
           "with some shocking surprises.\n");
    printf("\nCast: \nAnya Taylor-Joy\n Ralph Fiennes\n Nicholas Hoult\n John "
           "Leguizamo\n Janet McTeer\n");
    printf("\nRuntime: 1h 47m\n");
    printf("\nReviews: \n");
    printf("\n5/5 stars! So unpredictable, viciously funny thriller.\n");
    printf(
        "\n5/5 stars! Mark Mylod by far one of the best directors out their. "
        "The story, the acting, the picture overall was pure genius.");
    printf("\n5/5 stars! Best thriller this year. If you haven't seen it go "
           "now.\n");
  } else if (strcmp(word, "Black Panther: Wakanda Forever") == 0 ||
             strcmp(word, "black panther: wakanda forever") == 0) {
    printf("\nThere was a match!\n");
    printf("\n\t\t\t\t\tBlack Panther: Wakanda Forever (PG-13)\n");
    printf("Synopsis: \nQueen Ramonda, Shuri, M'Baku, Okoye and the Dora "
           "Milaje, fight to protect their nation from intervening world "
           "powers in the wake of King T'Challa's death. As the Wakandans "
           "strive to embrace their next chapter, the heroes must band "
           "together with the help of War Dog Nakia and Everett Ross and forge "
           "a new path for the kingdom of Wakanda.\n");
    printf("\nCast: \nLetitia Wright \nAngela Bassett\n Lupita Nyong'o\n Danai "
           "Gurira\n Winston Duke\n");
    printf("\nRuntime: 2h 41m \n");
    printf("\nReviews: \n");
    printf("\n\t4/5 stars! Following the first Black Panther was not an easy "
           "task, specially after loosing Chadwick. However they did an "
           "amazing job honoring his memory and picking up the story. \n");
    printf("\n\t4/5 stars! The performances are extermely personal and moving "
           "all around.\n");
    printf("\n\t5/5 stars! Namor by far best character. Wish he had more "
           "screen time tho.\n");

    char array[40] = {"Black Panther: Wakanda Forever"};
    printf("\n\n\tWhat next?\n");
    int nextPart = 0;
    printf("\n10- Buy Tickets\n11- Write a Review\n12- Go Back?\n");
    scanf("%d", &nextPart);
    if (nextPart == 10) {
      buyTickets(array);
    } else if (nextPart == 11) {
      writeReview(array);
    } else if (nextPart == 12) {
      mainScreen();
    } else {
      printf("Answer was not understood");
      mainScreen();
    }

  } else if (strcmp(word, "Avatar: The Way of Water") == 0 ||
             strcmp(word, "avatar: the way of water") == 0) {
    printf("\nThere was a match!\n");
    printf("\n\t\t\t\t\tAvatar: The Way of Water (PG-13)\n");
    printf("Synopsis:: \nAvatar The Way of Water begins to tell the story of "
           "the Sully family, Jake, Neytiri and their kids, the trouble that "
           "follows \n");
    printf("\nCast: \nZoe Saldana\nSigourney Weaver\nSam Worthington\nStephen "
           "Lang\n");
    printf("\nRuntime: 2h 42m\n");
    printf("\nComing to theaters on December 16th, 2022!\n");
  } else if (strcmp(word, "Ant-Man and the Wasp: Quantumania") == 0 ||
             strcmp(word, "ant-man and the wasp: quantumania") == 0) {
    printf("\nThere was a match!\n");
    printf("\n\t\t\tAnt-Man and The Wasp: Quantumania \n");
    printf("Synopsis:: \nSuper-Hero partners Scott Lang and Hope Van Dyne "
           "return to continue their adventures as Ant-Man and the Wasp. "
           "Together, with Hope's parents Hank Pym and Janet Van Dyne, the "
           "family finds themselves exploring the Quantum Realm. \n");
    printf("\nCast: \nPaul Rudd\nEvangeline Lilly\nMichael Douglas\nMichelle "
           "Pfeiffer\nKathryn Newton\n");
    printf("\nRuntime: to be determined\n");
    printf("\nComing to theaters on February 17th, 2023!\n");
  } else if (strcmp(word, "Empire of Light") == 0 ||
             strcmp(word, "empire of light") == 0) {
    printf("\nThere was a match!\n");
    printf("\n\t\t\t\t\tEmpire of Light (R)\n");
    printf("Synopsis:: \nSet in an English seaside town in the early 1980s, "
           "EMPIRE OF LIGHT is a powerful and poignant story about human "
           "connection and the magic of cinema. \n");
    printf("\nCast: \nOlivia Colman\nMicheal Ward\nToby Jones\nColin Firth\n");
    printf("\nRuntime: 1h 53m \n");
    printf("\nComing to theathers on December 9th, 2022!\n");
  } else if (strcmp(word, "I Wanna Dance With Somebody") == 0 ||
             strcmp(word, "i wanna dance with somebody") == 0) {
    printf("\nThere was a match!\n");
    printf("\n\t\t\t\t\tI Wanna Dance With Somebody (G)\n");
    printf("Synopsis:: \nNaomi Ackie stars as Whitney Houston in the musical "
           "biopic, which is based on the epic life and music of the iconic "
           "singer. The film will take audiences on an emotional, energetic "
           "journey through Houston's career and music. \n");
    printf("\nCast: \nNaomi Ackie\nStanley Tucci\nAshyon Sanders\nTamara "
           "Tunie\nNafessa Williams\n");
    printf("\nRuntime: 2h 22m \n");
    printf("\nComing to theaters on December 21st, 2022!\n");
  } else if (strcmp(word, "Father Stu: Reborn") == 0 ||
             strcmp(word, "father stu: reborn") == 0) {
    printf("\nThere was a match!\n");
    printf("\n\t\t\t\t\tFather Stu: Reborn (PG-13)\n");
    printf("Synopsis:: \n Based on a true story, Father Stu: Reborn is a PG-13 "
           "take on the honest, funny, and ultimately uplifting drama about a "
           "lost soul who finds his purpose in a most unexpected place.\n");
    printf("\nCast: \nMark Wahlberg\nMel Gibson\nJacki Weaver\nTheresa Ruiz\n");
    printf("\nRuntime: 2h 4m \n");
    printf("\nComing to theaters on December 9th, 2022!\n");
  } else {
    printf("\n\nThere was no match!\n\n");
    mainScreen();
  }
}

// function for buying tickets still working on
void buyTickets(char str[]) {
  printf("\n\nShowtimes for %s\n", str);
  printf("\nToday:\nAlamo Drafthouse Cinema: 10:40pm\nCinemark Tinseltown: "
         "10:35pm\n");
  printf("\nMon:\nAlamo Drafthouse Cinema: 4:10pm\t6:00pm\t10:25pm\nCinemark "
         "Tinseltown: 2:00pm\t4:55pm\t7:45pm\t10:35pm\nLUBBOCK Premire Lux 15: "
         "12:00pm\t2:25pm\t4:55pm\t7:25pm\t9:50pm\n");
  printf("\nTue:\nAlamo Drafthouse Cinema: 4:10pm\t6:00pm\t10:25pm\nCinemark "
         "Tinseltown: 2:00pm\t4:55pm\t7:45pm\t10:35pm\nLUBBOCK Premire Lux 15: "
         "12:00pm\t2:25pm\t4:50pm\t7:15pm\t9:50pm\n");
  printf("\nWed:\nAlamo Drafthouse Cinema: 4:10pm\t6:00pm\t10:25pm\nCinemark "
         "Tinseltown: 2:00pm\t4:55pm\t7:45pm\t10:35pm\nLUBBOCK Premire Lux 15: "
         "12:00pm\t2:25pm\t4:50pm\t7:15pm\t9:50pm\n");

  char day[6];
  char theater[50];
  char movieTime[10];
  printf("\nWhat day? ");
  printf(" What theather?(just the first word) ");
  printf(" What time?(without pm) ");
  scanf("%s %s %s", day, theater, movieTime);

  if (strcmp(day, "Today") == 0 || strcmp(day, "today")) {
    if (strcmp(theater, "Alamo") == 0 && (strcmp(movieTime, "10:40") == 0)) {
      printf("Theater: %s Day: %s Time: %s", theater, day, movieTime);
    } else if (strcmp(theater, "Cinemark") == 0 && strcmp(movieTime, "10:35")) {
      printf("Theater: %s Day: %s Time: %s", theater, day, movieTime);
    } else {
      printf("Somthing was not entered right.");
      mainScreen();
    }
  } else if (strcmp(day, "Mon") == 0 || strcmp(day, "mon") == 0) {
    if (strcmp(theater, "Alamo") == 0 &&
        ((strcmp(movieTime, "4:10") == 0) || (strcmp(movieTime, "6:00") == 0) ||
         (strcmp(movieTime, "10:25") == 0))) {
      printf("Theater: %s Day: %s Time: %s", theater, day, movieTime);
    } else if (strcmp(theater, "Cinemark") == 0 &&
               ((strcmp(movieTime, "2:00") == 0) ||
                (strcmp(movieTime, "4:55") == 0) ||
                (strcmp(movieTime, "7:45") == 0) ||
                strcmp(movieTime, "10:35") == 0)) {
      printf("Theater: %s Day: %s Time: %s", theater, day, movieTime);
    } else if (strcmp(theater, "Lubbock") == 0 &&
               ((strcmp(movieTime, "12:00") == 0) ||
                (strcmp(movieTime, "2:25") == 0) ||
                (strcmp(movieTime, "4:55") == 0) ||
                (strcmp(movieTime, "7:25") == 0) ||
                (strcmp(movieTime, "9:50") == 0))) {
      printf("Theater: %s Day: %s Time: %s", theater, day, movieTime);
    }
  } else if (strcmp(day, "Tue") == 0 || strcmp(day, "tue") == 0) {
    if (strcmp(theater, "Alamo") == 0 &&
        ((strcmp(movieTime, "4:10") == 0) || (strcmp(movieTime, "6:00") == 0) ||
         (strcmp(movieTime, "10:25") == 0))) {
      printf("Theater: %s Day: %s Time: %s", theater, day, movieTime);
    } else if (strcmp(theater, "Cinemark") == 0 &&
               (strcmp(movieTime, "2:00" == 0) || strcmp(movieTime, "4:55" == 0) || strcmp(movieTime, "7:45") == 0) ||
                strcmp(movieTime, "10:35") == 0) {
      printf("Theater: %s Day: %s Time: %s", theater, day, movieTime);
    } else if (strcmp(theater, "Lubbock") == 0 &&
               ((strcmp(movieTime, "12:00") == 0) ||
                (strcmp(movieTime, "2:25") == 0) ||
                (strcmp(movieTime, "4:55") == 0) ||
                (strcmp(movieTime, "7:25") == 0) ||
                (strcmp(movieTime, "9:50") == 0))) {
      printf("Theater: %s Day: %s Time: %s", theater, day, movieTime);
    }
  } else if (strcmp(day, "Wed") == 0 || strcmp(day, "wed") == 0) {
    if (strcmp(theater, "Alamo") == 0 &&
        ((strcmp(movieTime, "4:10") == 0) || (strcmp(movieTime, "6:00") == 0) ||
         (strcmp(movieTime, "10:25") == 0))) {
      printf("Theater: %s Day: %s Time: %s", theater, day, movieTime);
    } else if (strcmp(theater, "Cinemark") == 0 &&
               ((strcmp(movieTime, "2:00") == 0) ||
                (strcmp(movieTime, "4:55") == 0) ||
                (strcmp(movieTime, "7:45") == 0) ||
                strcmp(movieTime, "10:35") == 0)) {
      printf("Theater: %s Day: %s Time: %s", theater, day, movieTime);
    } else if (strcmp(theater, "Lubbock") == 0 &&
               ((strcmp(movieTime, "12:00") == 0) ||
                (strcmp(movieTime, "2:25") == 0) ||
                (strcmp(movieTime, "4:55") == 0) ||
                (strcmp(movieTime, "7:25") == 0) ||
                (strcmp(movieTime, "9:50") == 0))) {
      printf("Theater: %s Day: %s Time: %s", theater, day, movieTime);
    }
  } else {
    printf("\nInput was not correct.");
    mainScreen();
  }

  int total_amount;
  int cost = 8;
  int numT = 0;
  printf("\n\nHow many tickets do you want to buy? ");
  scanf("%d", &numT);
  total_amount = numT * cost;
  if (numT < 10) {
    printf("\n\tCart:");
    printf("\nMovie: %s \nMovie Theater: %s \nDay and Time: %s %spm \nNumber "
           "of Tickets: %d",
           str, theater, day, movieTime, numT);
  } else {
    printf("\nOnly allowed to buy max of 10 tickets. For now you can get 10.");
    numT = 10;
    printf("\nMovie: %s \nMovie Theater: %s \nDay and Time: %s %spm \nNumber "
           "of Tickets: %d",
           str, theater, day, movieTime, numT);
  }
  printf("\nYour Total Price is: $ %d", total_amount);
  printf("\n\nLet's checkout!");
  checkout();
}
static void checkout() {
  int input;
  printf("Will you be paying with \n1.Debit\n2.Credit\n3.Paypal\n");
  scanf("%d", &input);
  if (input == 1) {
    debit();
  } else if (input == 2) {
    credit();
  } else if (input == 3) {
    printf("................Redirecting to PayPal................");
    paypal();
  } else {
    printf("\nNumber Entered Invalid. Try Again!\n");
    menuScreen();
  }
}
static void debit() {
  int cardNumber;
  int expDate;
  int cvvNumber;
  int numberRead;
  int month;
  int year;
  int input;

  printf("\nEnter 16 digit Card Number:\t");
  // fprint(fp, "%s", &cardNumber);
  numberRead = scanf("%d", &cardNumber);
  while (numberRead != 1) {
    printf("That is not a valid card number. \n");
    scanf("%*[^\n]");
    printf("\nEnter 16 Digit Card Number:\t");
  }

  do {
    printf("\nEnter Card Expiration Date:\t");
    printf("\nEnter the Month first (1-12): ");
    scanf("%d", &month);
    if (month < 1 || month > 12)
      printf("\nPlease enter a valid month");
  } while (month < 1 || month > 12);
  {
    printf("\nEnter the Year of the Card:");
    scanf("%d", &year);
  }

  printf("\nEnter CVV Number:\t");
  // fprint(fp, "%s", &cvvNumber);
  numberRead = scanf("%d", &cvvNumber);
  while (numberRead != 1) {
    printf("That is not a valid CVV Number. \n");
    scanf("%*[^\n]");
    printf("\nEnter CVV Number:\t");
    numberRead = scanf("%d", &cvvNumber);
  }

  printf("\nPress 1 to Confirm Payment\nPress 2 to Return to Menu Screen\n");
  scanf("%d", &input);
  if (input == 1) {
    printf("\nPayment Successful");
    barcode();
  } else if (input == 2) {
    mainScreen();
  } else {
    printf("\nNumber Entered Invalid. Try Again!\n");

    menuScreen();
  }
}
static void credit() {
  int cardNumber;
  int expDate;
  int cvvNumber;
  int numberRead;
  int month;
  int year;
  int input;

  printf("\nEnter 16 digit Card Number:\t");
  // fprint(fp, "%s", &cardNumber);
  numberRead = scanf("%d", &cardNumber);
  while (numberRead != 1) {
    printf("That is not a valid card number. \n");
    scanf("%*[^\n]");
    printf("\nEnter 16 Digit Card Number:\t");
  }

  do {
    printf("\nEnter Card Expiration Date:\t");
    printf("\nEnter the Month first (1-12): ");
    scanf("%d", &month);
    if (month < 1 || month > 12)
      printf("\nPlease enter a valid month");
  } while (month < 1 || month > 12);
  {
    printf("\nEnter the Year of the Card:");
    scanf("%d", &year);
  }

  printf("\nEnter CVV Number:\t");
  // fprint(fp, "%s", &cvvNumber);
  numberRead = scanf("%d", &cvvNumber);
  while (numberRead != 1) {
    printf("That is not a valid CVV Number. \n");
    scanf("%*[^\n]");
    printf("\nEnter CVV Number:\t");
    numberRead = scanf("%d", &cvvNumber);
  }

  printf("\nPress 1 to Confirm Payment\nPress 2 to Return to Menu Screen\n");
  scanf("%d", &input);
  if (input == 1) {
    printf("\nPayment Successful");
    barcode();
  } else if (input == 2) {
    mainScreen();
  } else {
    printf("\nNumber Entered Invalid. Try Again!\n");
    menuScreen();
  }
}

static void paypal() {
  {
    int pemail;

    printf("\nEnter Paypal Email\t");
    scanf("%s", pemail);
  }
  {}
  int input;
  printf("\nSuccessful Login");
  printf("\n................Redirecting back to MBS................");
  scanf("%d", &input);
  printf("\nPayment Successful");
  barcode();
}

static void barcode() {
  system("clear");
  srand(time(0));
  printf("\n\nYour Barcode Number Is\n");
  rand_num = rand();
  printf("%d\n", rand_num);
  printf("Present this number upon Arrival");

  int code;
  printf("\nPress 0 to Return to Main Screen:  ");
  scanf("%d", &code);
  if (code == 0) {
    newMainScreen();
  } else {
    newMainScreen();
  }
}

void writeReview(char str[]) {

  char review[100];
  printf("\nWrite a review for %s ", str);
  printf(": ");
  scanf("%s", review);
}
static void newMainScreen() {
  // need to be logined in or register to work
  system("clear");
  printf("\n\t\tMain Screen\n");
  printf("0- Browse Current Movie Catalog\n");
  printf("1- Browse Upcoming Movie Catalog\n");
  printf("2- Search Movies\n");
  printf("3- Admin\n");
  printf("4- View Barcode\n");

  int numInput;
  printf("\nEnter your choice :: ");
  scanf("%d", &numInput);

  switch (numInput) {
  case (0):
    {
    char oldMovie[][40] = {"The Polar Express", "Bones and All","Strange World", "The Menu", "Black Panther: Wakanda Forever"};
    int row = 5;
    currentMovie(oldMovie, 5);
    break;     
    }
  case (1):
    upcomingMovie();
    break;
  case (2):
    search();
    break;
  case (3):
    admin();
    break;
  case (4):
    barcodeReciept();
    break;
  }
}

static void barcodeReciept() {
  printf("%d\n", rand_num);
  int code;
  printf("\nPress 0 to Return to Main Screen:  ");
  scanf("%d", &code);
  if (code == 0) {
    newMainScreen();
  } else {
    newMainScreen();
  }
}
static void manageShow() {
  printf("0- View Showings\n");
  printf("1- Add Showings\n");
  printf("2- Remove Showings\n");

  int numInput;
  printf("\nEnter your choice :: ");
  scanf("%d", &numInput);
  switch (numInput) {
  case (0):
    view();
    break;
  case (1):
    {
    char oldMovie[][40] = {"The Polar Express", "Bones and All","Strange World", "The Menu", "Black Panther: Wakanda Forever"};
    int row = 5;
    add(oldMovie, row);
    break;
    }
  case (2):
    {    
      char oldMovie[][40] = {"The Polar Express", "Bones and All","Strange World", "The Menu", "Black Panther: Wakanda Forever"};
    int row = 5;
    removee(oldMovie, row);
    break;    
    }
  }
}
static void view() {
  printf("0- Current Movie Catalog\n");
  printf("1- Upcoming Movie Catalog\n");
  printf("2- Search Movies\n");

  int numInput;
  printf("\nEnter your choice :: ");
  scanf("%d", &numInput);
  switch (numInput) {
  case (0):
    {
    char oldMovie[][40] = {"The Polar Express", "Bones and All",
                           "Strange World", "The Menu",
                           "Black Panther: Wakanda Forever"};
    int row = 5;
    currentMovie(oldMovie, row);
    break;
    }
  case (1):
    upcomingMovie();
    break;
  case (2):
    search();
    break;
  }
}
void add(char update[][40], int y) {

  y= 5;

  printf("\n\n\t\tNEW CONTENT ADDED! \nViolent Night has just been released to "
         "view.\n");
  char answer[6];
  printf("\nWould you like to add the movie to current movie list? (Y/N) ");
  scanf("%s", &answer);

  // char addMovie[] = "Violent Night";
  // update[6][40] = strcat(update, addMovie);

  if (strcmp(answer, "Y") == 0 || strcmp(answer, "y") == 0) {
    printf("\nSuccessfully added!\n");
    y++;
    char updatedArray[6][40] = {"The Polar Express",
                                "Bones and All",
                                "Strange World",
                                "The Menu",
                                "Black Panther: Wakanda Forever","Violent Night"};
    currentMovie(updatedArray, y);
  } 
  else if (strcmp(answer, "N") == 0 || strcmp(answer, "n") == 0)
  {
    printf("\nNothing has been added!\n");
     char updatedArray[6][40] = {"The Polar Express",
                                "Bones and All",
                                "Strange World",
                                "The Menu",
                                "Black Panther: Wakanda Forever",};
    currentMovie(updatedArray, y);
  }
}

void removee(char update[][40], int y) { 
  printf("\n\n\tCONTENT NEEDS TO BE REMOVED! \n Black Panther: Wakanda Forever needs to be removed.\n");
  char answer[6];
  printf("\nWould you like to remove the movie to current movie list? (Y/N) ");
  scanf("%s", &answer);

  if(y ==  5)
  {
    if (strcmp(answer, "Y") == 0 || strcmp(answer, "y") == 0) 
    {
    y--;
    char updatedArray[4][40] = {"The Polar Express",
                                "Bones and All",
                                "Strange World",
                                "The Menu"};
    printf("\nSuccessfully removed!\n");
    currentMovie(updatedArray, y);
      }
    else {
    printf("\nNothing has been removed!\n");
     char updatedArray[5][40] = {"The Polar Express",
                                "Bones and All",
                                "Strange World",
                                "The Menu",
                                "Black Panther: Wakanda Forever"};
      y= 5;
    currentMovie(updatedArray, y);
  }
    }
  else if(y < 5)
  {
    printf("\nThe movie has already been removed\n");

    char updatedArray[4][40] = {"The Polar Express",
                                "Bones and All",
                                "Strange World",
                                "The Menu"};
    y= 4;
    
    currentMovie(updatedArray, y); 
  }
  
  
  }



















