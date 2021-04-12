#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

typedef enum Decision {ADMIS, REFUSE, AJOURNE} Decision;

typedef struct candidat
{
    int NCIN;
    char NOM[15];
    char PRENOM[15];
    int AGE;
    int NOTE[10];
    Decision DECISION;
} candidat;

void saisie()
{
    candidat eleve;
    int i, dec = 0;
    double sum = 0;
    char *decis;

    FILE *concours;
    concours = fopen("concours.txt", "a");

    if(concours == NULL)
		printf("\n\tERREUR D'OUVERTURE");
    else
	{
        printf("\tENTREZ LE NUMERO DE CIN : ");
        scanf("%d", &eleve.NCIN);
        printf("\n\tENTREZ LE NOM : ");
        scanf("%s", &eleve.NOM);
        printf("\n\tENTREZ LE PRENOM : ");
        scanf("%s", &eleve.PRENOM);

        do
        {
			printf("\n\tENTREZ L'AGE : ");
        	scanf("%d", &eleve.AGE);
		} while(eleve.AGE < 10 || eleve.AGE > 40);

        printf("\n\tENTREZ LES NOTES DE CHAQUE MATIERE \n");
        for(i = 0; i < 10; i++)
			do
			{
				printf("\t\tNOTE %d : ", i+1);
            	scanf("%d", &eleve.NOTE[i]);
			} while (eleve.NOTE[i] < 0 || eleve.NOTE[i] > 20);

    	for(i = 0; i < 10; i++)
		{
			sum = sum + eleve.NOTE[i];

        	if(eleve.NOTE[i] < 10)
				dec = 1;
		}

        if(sum >= 100 && dec == 0)
			decis = "ADMIS";
        else if(sum >= 100 && dec == 1)
			decis = "AJOURNE";
        else
			decis = "REFUSE";

        fprintf(concours, "%d ; %s ; %s ; %d ;" ,eleve.NCIN, &eleve.NOM , &eleve.PRENOM, eleve.AGE);
        for(i = 0; i < 10; i++)
			fprintf(concours, " %d ;", eleve.NOTE[i]);
        fprintf(concours, " %s\n", decis);

        fclose(concours);

		printf("\n\n\tCONFIRMATION D'ENREGISTREMENT DU CANDIDAT!\n\tVISUALISEZ LE RESULTAT DANS LE FICHIER \"concours.txt\"");
    }
}

void supprimer(int NCIN)
{
	candidat eleve;
	int c, n = 0, k;
    char texte[100];

    FILE *concours;
    FILE *selection;

    concours = fopen("concours.txt", "r");
	selection = fopen("concours.select", "a");

	if(concours == NULL || selection == NULL)
		printf("\n\tERREUR D'OUVERTURE");
	else
	{
	    while((c = fgetc(concours)) != EOF)
	        if(c == '\n')
	            n++;
	    rewind(concours);

	    for(c = 0; c < n; c++)
		{
	        fscanf(concours, "%d", &eleve.NCIN);
	        rewind(concours);

	        for(k = 0; k <= c; k++)
	        	fgets(texte, 100, concours);
	    	if(eleve.NCIN != NCIN)
	        	fprintf(selection, "%s", texte);
		}

	    fclose(concours);
	    fclose(selection);

	    remove("concours.txt");
	    rename("concours.select", "concours.txt");

	    printf("\n\tCONFIRMATION DE SUPPRESSION DU CANDIDAT!\n\tVISUALISEZ LE RESULTAT DANS LE FICHIER \"concours.txt\"");
	}

	printf("\n\n");
	admis();
	printf("\n\n");
	attente();
}

void admis()
{
    candidat eleve;
    int c, n = 0;
    char texte[100], *decis;

    FILE *concours;
    FILE *admis;
    concours = fopen("concours.txt", "r");
    admis = fopen("admis.txt", "w");

    if(concours == NULL || admis == NULL)
    	printf("\n\tERREUR D'OUVERTURE");
    else
	{
        while((c = fgetc(concours)) != EOF)
            if(c == '\n')
                n++;
        rewind(concours);

        for(c = 0; c < n; c++)
		{
        	fgets(texte, 100, concours);
        	decis = strstr(texte, "ADMIS");
        	if(decis != NULL)
            	fprintf(admis, "%s", texte);
        }

       fclose(concours);
       fclose(admis);

       printf("\tMODIFICATION DE LA LISTE D'ADMIS EFFECTUEE!\n\tVISUALISEZ LE RESULTAT DANS LE FICHIER \"admis.txt\"");
    }
}

void attente()
{
    candidat eleve;
    int c, n = 0, k;
    char texte[100];

    FILE *admis;
    FILE *attente;
    admis = fopen("admis.txt", "r");
    attente = fopen("attente.txt", "w");

	if(admis == NULL || attente == NULL)
    	printf("\n\tERREUR D'OUVERTURE");
    else
	{
	    while((c = fgetc(admis)) != EOF)
	    	if(c == '\n')
	            n++;
	    rewind(admis);

	    for(c = 0; c < n; c++)
		{
	        fscanf(admis, "%d ; %s ; %s ; %d", &eleve.NCIN, &eleve.NOM , &eleve.PRENOM, &eleve.AGE);
	        rewind(admis);

	        for(k = 0; k <= c; k++)
	        	fgets(texte, 100, admis);
	    	if(eleve.AGE > 20)
				fprintf(attente, "%s", texte);
	    }

	    fclose(admis);
	    fclose(attente);

	    printf("\tMODIFICATION DE LA LISTE D'ATTENTE EFFECTUEE!\n\tVISUALISEZ LE RESULTAT DANS LE FICHIER \"attente.txt\"");
	}
}

double statistiques(Decision dec)
{
	char decis[8] = {0}, texte[100], *d;
    int c;
	double tot = 0, eff = 0;

    FILE *concours;
    concours = fopen("concours.txt", "r");

    if(concours == NULL)
    	printf("\n\tERREUR D'OUVERTURE");
    else
	{
	    if(dec == 0)	
	        strcpy(decis, "ADMIS");
	    else if(dec == 1)
	        strcpy(decis, "REFUSE");
	    else
	        strcpy(decis, "AJOURNE");

	    while((c = fgetc(concours)) != EOF)
	    	if(c == '\n')
	        	tot++;
	    rewind(concours);

	    for(c = 0; c < tot; c++)
		{
	        fgets(texte, 100, concours);
	        d = strstr(texte, decis);
	        if(d != NULL)
	            eff++;
	    }

	    fclose(concours);
	}

	return (eff/tot)*100;
}

void supprimer_20()
{
    candidat eleve;
    int c, n = 0, k;
    char texte[100];

    FILE *admis;
    FILE *test;
    admis = fopen("admis.txt", "r");
    test = fopen("admis.test", "a");

	if(admis == NULL || test == NULL)
		printf("\n\tERREUR D'OUVERTURE");
	else
	{
	    while((c = fgetc(admis)) != EOF)
	    	if(c == '\n')
	        	n++;
		rewind(admis);

	    for(c = 0; c < n; c++)
	    {
	        fscanf(admis, "%d ; %s ; %s ; %d", &eleve.NCIN, &eleve.NOM , &eleve.PRENOM, &eleve.AGE);
	        rewind(admis);

	        for(k = 0; k <= c; k++)
	        	fgets(texte, 100, admis);
	    	if(eleve.AGE <= 20)
	        	fprintf(test, "%s", texte);
	    }

	    fclose(admis);
	    fclose(test);

	    remove("admis.txt");
	    rename("admis.test", "admis.txt");

	    printf("\n\tCONFIRMATION DE SUPPRESSION DES CANDIDATS DE PLUS DE 20 ANS!\n\tVISUALISEZ LE RESULTAT DANS LE FICHIER \"admis.txt\"");
	}
}

void recherche(int NCIN)
{
    candidat eleve;
    int c, n = 0, k, test = 0;
    char texte[100];

    FILE *concours;
    concours = fopen("concours.txt", "r");

	if(concours == NULL)
		printf("\n\tERREUR D'OUVERTURE");
	else
	{
	    while((c = fgetc(concours)) != EOF)
	    	if(c == '\n')
	            n++;
	    rewind(concours);

		printf("\n\tRESULTATS DE LA RECHERCHE : \n");
	    for(c = 0; c < n; c++)
		{
	        fscanf(concours, "%d", &eleve.NCIN);
	        rewind(concours);

	        for(k = 0; k <= c; k++)
	        	fgets(texte, 100, concours);
	    	if(eleve.NCIN == NCIN)
	    	{
	    		test = 1;
				printf("\n\t%s", texte);
			}
	    }
	    if(test == 0)
			printf("\n\tAUCUN CANDIDAT NE CORRESPOND A L'IDENTIFIANT %d", NCIN);

	    fclose(concours);
	}
}

void presentation()
{
    printf("\t\t 1. SAISIR / AJOUTER\n");
    printf("\t\t 2. MODIFIER\n");
    printf("\t\t 3. SUPPRIMER\n");
    printf("\t\t 4. ADMIS\n");
    printf("\t\t 5. ATTENTE\n");
    printf("\t\t 6. STATISTIQUES\n");
    printf("\t\t 7. SUPPRIMER 20 ANS\n");
    printf("\t\t 8. ORDRE DE MERITE\n");
    printf("\t\t 9. RECHERCHE\n");
}

int main()
{
	int choix_1, choix_2, n, conf;

	printf("\n\n\t\t- - - - - - - GESTION DU CONCOURS - - - - - - -\n\n\n");

	do
	{
		printf("\n\n");
		presentation();
		printf("\n\t\t\tCHOISISSEZ L'OPERATION A EXECUTER : ");
        scanf("%d", &choix_2);
		printf("\n--------------------------------------------------------------------------------------------------------\n");

        switch(choix_2)
		{
        	case 1 : saisie(); getch(); break;

            case 2 : printf("\tDESOLE, CETTE FONCTION N'EST PAS DISPONIBLE POUR L'INSTANT :("); getch(); break;

            case 3 : printf("\tENTREZ LE NUMERO DE CIN DU CANDIDAT A RETIRER DE LA LISTE : ");
                	 scanf("%d", &n);
                	 printf("\n\tSOUHAITEZ-VOUS VRAIMENT CONTINUER? ATTENTION CETTE ACTION EST IRREVERSIBLE\n\n\t\t1. OUI \n\t\tAPPUYEZ SUR UN AUTRE CHIFFRE POUR ANNULER \n\n\t\t\tCHOISSISEZ L'ACTION A EXECUTER : ");
                	 scanf("%d", &conf);
                	 if(conf == 1)supprimer(n);
                	 else printf("\n\tSUPPRESSION ANNULEE");
                	 getch(); break;

            case 4 : admis(); getch(); break;

            case 5 : attente(); getch(); break;

            case 6 : printf("\t1. POURCENTAGE ADMIS \n\t2. POURCENTAGE REFUSE \n\t3. POURCENTAGE AJOURNE\n\n\t\t\tCHOISISSEZ L'OPERATION A EXECUTER : ");
                     scanf("%d", &n);
                     if(n == 1) printf("\n\tLE POURCENTAGE DE CANDIDATS ADMIS EST : %f POUR CENT", statistiques(n-1));
					 else if(n == 2) printf("\n\tLE POURCENTAGE DE CANDIDATS REFUSES EST : %f POUR CENT", statistiques(n-1));
                     else if(n == 3) printf("\n\tLE POURCENTAGE DE CANDIDATS AJOURNES EST : %f POUR CENT", statistiques(n-1));
					 else printf("\n\n\n\tVALEUR INVALIDE!");
					 getch(); break;

            case 7 : printf("\tSOUHAITEZ-VOUS VRAIMENT CONTINUER? ATTENTION CETTE ACTION EST IRREVERSIBLE\n\n\t\t1. OUI \n\t\tAPPUYEZ SUR UN AUTRE CHIFFRE POUR ANNULER \n\n\t\t\tCHOISSISEZ L'ACTION A EXECUTER : ");
                	 scanf("%d", &conf);
                	 if(conf == 1)supprimer_20();
					 else printf("\n\tSUPPRESSION ANNULEE");
					 getch(); break;

            case 8 : printf("\tDESOLE, CETTE FONCTION N'EST PAS DISPONIBLE POUR L'INSTANT :("); getch(); break;

            case 9 : printf("\tENTREZ LE NUMERO DE CIN A RECHERCHER : ");
            		 scanf("%d",&n);
                	 recherche(n);
                	 getch(); break;

			default : printf("\tVALEUR INVALIDE!"); getch(); break;
		}

        printf("\n--------------------------------------------------------------------------------------------------------\n\n\n\n\n\n\tDESIREZ-VOUS EFFECTUER UN AUTRE TRAITEMENT?\n\n\t\t1. OUI \n\t\tAPPUYEZ SUR UN AUTRE CHIFFRE POUR SORTIR\n\n \t\t\tCHOISISSEZ L'OPERATION A EXECUTER : ");
        scanf("%d", &choix_1);
	} while (choix_1 == 1);

	return 0;
}
