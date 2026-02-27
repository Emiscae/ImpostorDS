#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <random>
#include <ctime>

#define MAX_NOMES 10
#define TAM_NOME 21
#define NUM_TEMAS 5

using namespace std;

mt19937 engine(time(NULL));

char nomes[MAX_NOMES][TAM_NOME];
int totalNomes = 0;
int click = 0;
int selecTema = 0;
bool temaSelec[NUM_TEMAS] = {false};

const char *comidas[37] = {"pao", "queijo", "presunto", "cachorro-quente", "coxinha",
        "pastel", "esfiha", "bolo", "biscoito", "pipoca", "panqueca", "omelete", "strogonoff",
        "feijoada", "farofa", "pure de batata", "arroz doce", "brigadeiro", "pudim", "sorvete",
        "churrasco", "peixe frito", "peixe assado", "camarao", "risoto", "yakisoba", "sushi",
        "taco", "burrito", "empada", "sopa", "croissant", "cereal", "iogurte", "fruta", "vitamina", "mingau"
    };
const char *paises[20] = {"Brasil", "Estados Unidos", "Canada", "Mexico", "Argentina", "Chile", "Colombia", "Peru", "Alemanha", "Franca", "Italia", "Espanha", "Portugal", "Reino Unido", "Japao", "China", "Coreia do Sul", "India", "Australia", "Africa do Sul"};
const char *emocoes[15] = {"alegria", "tristeza", "raiva", "medo", "ansiedade", "calma", "empolgacao", "entusiasmo", "frustracao", "culpa", "vergonha", "orgulho", "amor", "solidao", "esperanca"};
const char *profissoes[20] = {"programador", "professor", "medico", "enfermeiro", "engenheiro", "advogado", "arquiteto", "psicologo", "motorista", "cozinheiro", "vendedor", "designer", "jornalista", "contador", "policial", "bombeiro", "dentista", "mecanico", "eletricista", "administrador"};
const char *animais[50] = {"rato", "camundongo", "hamster", "peixe", "tubarao", "baleia", "golfinho", "polvo", "caranguejo", "lagosta", "tartaruga", "cobra", "lagarto", "jacare", "sapo", "ratoeira", "borboleta", "abelha", "formiga",
        "aranha", "mosca", "mosquito", "passaro", "pombo", "galinha-dangola", "aguia", "coruja",
        "papagaio", "canario", "pato", "urso", "lobo", "raposa", "leao", "tigre", "elefante",
        "girafa", "zebra", "rinoceronte", "hipopotamo", "macaco", "gorila", "chimpanze"
    };
char nome[TAM_NOME];


PrintConsole topScreen;
PrintConsole bottomScreen;

void jogo();
void menu();
void gerencPart();
void adicPart();
void gerencTemas();
void rodada(int tema, int palavra, int palavra2, int impostor);
void secreto();
int aleatorio(int x, int y);

void testeAleatorio();

int main(){
    videoSetModeSub(MODE_0_2D);
    vramSetBankC(VRAM_C_SUB_BG);
    consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
    
    videoSetMode(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    consoleInit(&topScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);

    srand(time(NULL));

    // inicializa nomes
    for(int i = 0; i < MAX_NOMES; i++){
        nomes[i][0] = '\0';
    }
    
    

    menu();

    while(pmMainLoop()){
        swiWaitForVBlank();
    }
    return 0;
}
void menu(){
    int selecao = 0;
    int modoSecreto = aleatorio(0, 100);

    while(pmMainLoop()){
        scanKeys();
        int keys = keysDown();

        consoleSelect(&topScreen);
        consoleClear();

        iprintf("\n\n\n\n\n\n\n\n\n  Pressione A para selecionar");

        consoleSelect(&bottomScreen);
        consoleClear();

        iprintf("\n        Impostor - Menu\n\n\n");
        for(int i = 0; i < 3; i++){
            if(selecao == i){
                iprintf(" > ");
            }
            else{
                iprintf("   ");
            }
            switch(i){
                case 0:
                    iprintf("Gerenciar participantes\n\n");
                    break;
                case 1:
                    iprintf("Gerenciar temas\n\n");
                    break;
                case 2:
                    iprintf("Comecar\n\n");
            }
        }

        

        if(keys & KEY_A){
            if(modoSecreto == 50) secreto();
            else{
                switch(selecao){
                    case 0:
                        gerencPart();
                        break;
                    case 1:
                        gerencTemas();
                        break;
                    case 2:
                        jogo();
                        break;
                }
            }
            
        }
        if(keys & KEY_L && KEY_R) secreto();
        if(keys & KEY_UP && selecao > 0) selecao--;
        if(keys & KEY_DOWN && selecao < 2) selecao++;

        swiWaitForVBlank();
    }
}
void gerencPart(){
    while(pmMainLoop()){
        scanKeys();
        int keys = keysDown();

        consoleSelect(&bottomScreen);
        consoleClear();

        iprintf("\n         Participantes\n\n");

        for(int i = 0; i < MAX_NOMES; i++){
            iprintf(" - ");
            if(nomes[i][0] != '\0'){
                iprintf("%s", nomes[i]);
            }
            iprintf("\n\n");
        }

        consoleSelect(&topScreen);
        consoleClear();
        iprintf("\n\n\n\n\n\n\n\n   Pressione Y para adicionar\n         participantes");
        iprintf("\n\n   Pressione B para retornar");

        if(keys & KEY_B) return;
        if(keys & KEY_Y) adicPart();

        swiWaitForVBlank();
    }
}
void adicPart(){
    consoleSelect(&topScreen);
    consoleClear();
    iprintf("\n\n\n\n\n\n\n\n\n Pressione A para selecionar\n\n Pressione Start para confirmar");
        

    consoleSelect(&bottomScreen);
    char letras[26] = {
        'A','B','C','D','E','F','G','H','I','J',
        'K','L','M','N','O','P','Q','R','S','T',
        'U','V','W','X','Y','Z'
    };

    
    int tamanho = 0;
    int selecao = 0;

    nome[0] = '\0';

    while(pmMainLoop()){
        scanKeys();
        int keys = keysDown();

        consoleSelect(&bottomScreen);
        consoleClear();

        iprintf("\n Digite um nome:\n\n");
        iprintf(" Nome: %s\n\n", nome);

        for(int i = 0; i < 26; i++){
            if(i == selecao)
                iprintf(" >%c", letras[i]);
            else
                iprintf("  %c", letras[i]);

            if(i == 9 || i == 19)
                iprintf("\n\n");
        }

        if(keys & KEY_UP && selecao > 9) selecao -= 10;
        if(keys & KEY_DOWN && selecao < 16) selecao += 10;
        if(keys & KEY_LEFT && selecao > 0) selecao--;
        if(keys & KEY_RIGHT && selecao < 25) selecao++;

        // adicionar letra
        if(keys & KEY_A){
            if(tamanho < TAM_NOME - 1){
                nome[tamanho++] = letras[selecao];
                nome[tamanho] = '\0';
            }
        }

        // apagar letra
        if(keys & KEY_B && tamanho > 0){
            tamanho--;
            nome[tamanho] = '\0';
        }

        // confirmar nome
        if(keys & KEY_START){
            if(totalNomes < MAX_NOMES && tamanho > 0){
                strcpy(nomes[totalNomes], nome);
                totalNomes++;
            }
            return;
        }

        swiWaitForVBlank();
    }
}
void gerencTemas(){
    consoleSelect(&bottomScreen);
    
    int selecao = 0;

    while(pmMainLoop()){
        consoleSelect(&topScreen);
        consoleClear();
        iprintf("\n\n\n\n\n\n\n\n\n  Pressione A para selecionar\n\n  Pressione B para retornar");
        

        consoleSelect(&bottomScreen);
        consoleClear();
        scanKeys();

        int key = keysDown();

        iprintf("\n         Escolher temas\n\n\n");

        for(int i = 0; i < NUM_TEMAS; i++){
            if(selecao == i){
                iprintf(" >");
            }
            else{
                iprintf("  ");
            }
            if(temaSelec[i] == true){
                iprintf(" [x] ");
            }
            else{
                iprintf(" [ ] ");
            }
            switch(i){
                case 0:
                    iprintf("Comidas");
                    break;
                case 1:
                    iprintf("Paises");
                    break;
                case 2:
                    iprintf("Emocoes");
                    break;
                case 3:
                    iprintf("Profissoes");
                    break;
                case 4:
                    iprintf("Animais");
                    break;
            }
            iprintf("\n\n");
        }

        if(key & KEY_A) temaSelec[selecao] = !temaSelec[selecao];
        if(key & KEY_B) return;
        if(key & KEY_UP && selecao > 0) selecao--;
        if(key & KEY_DOWN && selecao < NUM_TEMAS - 1) selecao++;

        swiWaitForVBlank();
    }
}
int aleatorio(int x, int y){
    uniform_int_distribution<int> dist(x, x + y - 1);
    return dist(engine);
}

void rodada(int tema, int palavra, int palavra2, int impostor){
    consoleSelect(&bottomScreen);
    

    for(int i = 0; i < totalNomes; i++){
        while(pmMainLoop()){
            consoleSelect(&topScreen);
            consoleClear();

            iprintf("\n\n\n\n\n\n\n\n     Para ver sua palavra\n     segure Y");
            iprintf("\n\n  Para prosseguir pressione A");

            consoleSelect(&bottomScreen);
            consoleClear();

            scanKeys();
            int key = keysHeld();
            int key2 = keysDown();

            iprintf("\n\n          %s\n\n\n\n\n\n\n\n", nomes[i]);

            if(key & KEY_Y && i == impostor){
                switch(tema){
                    case 0:
                        iprintf("          %s", comidas[palavra2]);
                        break;
                    case 1:
                        iprintf("          %s", paises[palavra2]);
                        break;
                    case 2:
                        iprintf("          %s", emocoes[palavra2]);
                        break;
                    case 3:
                        iprintf("          %s", profissoes[palavra2]);
                        break;
                    case 4:
                        iprintf("          %s", animais[palavra2]);
                        break;
                }
            }
            else if(key & KEY_Y && i != impostor){
                switch(tema){
                    case 0:
                        iprintf("          %s", comidas[palavra]);
                        break;
                    case 1:
                        iprintf("          %s", paises[palavra]);
                        break;
                    case 2:
                        iprintf("          %s", emocoes[palavra]);
                        break;
                    case 3:
                        iprintf("          %s", profissoes[palavra]);
                        break;
                    case 4:
                        iprintf("          %s", animais[palavra]);
                        break;
                }
            }
            if(key2 & KEY_A) break;

            swiWaitForVBlank();
        }
    }
    return;
}
void jogo(){
    int palavra = 0;
    int palavra2 = 0;
    int tema = 100;
    int holder;
    int impostor;

    consoleSelect(&bottomScreen);
    consoleClear();

    consoleSelect(&topScreen);
    consoleClear();

    iprintf("\n\n\n\n\n\n\n         Caregando...");
    
    do{
        holder = aleatorio(0, NUM_TEMAS);
        if(temaSelec[holder] == true){
            tema = holder;
        }
    }while(tema == 100);
    impostor = aleatorio(0, totalNomes);

    consoleClear();
    

    switch(tema){
        case 0:
            palavra = aleatorio(0, 36);
            palavra2 = aleatorio(0, 36);
            rodada(tema, palavra, palavra2, impostor);
            //iprintf("\n\n  Tema: %d\n  Palavra: %s\n  Palavra2: %s\n  Impostor: %s", tema, comidas[palavra], comidas[palavra2], nomes[impostor]);
            break;
        case 1:
            palavra = aleatorio(0, 19);
            palavra2 = aleatorio(0, 19);
            rodada(tema, palavra, palavra2, impostor);
            //iprintf("\n\n  Tema: %d\n  Palavra: %s\n  Palavra2: %s\n  Impostor: %s", tema, paises[palavra], paises[palavra2], nomes[impostor]);
            break;
        case 2:
            palavra = aleatorio(0, 14);
            palavra2 = aleatorio(0, 14);
            rodada(tema, palavra, palavra2, impostor);
            //iprintf("\n\n  Tema: %d\n  Palavra: %s\n  Palavra2: %s\n  Impostor: %s", tema, emocoes[palavra], emocoes[palavra2], nomes[impostor]);
            break;
        case 3:
            palavra = aleatorio(0, 19);
            palavra2 = aleatorio(0, 19);
            rodada(tema, palavra, palavra2, impostor);
            //iprintf("\n\n  Tema: %d\n  Palavra: %s\n  Palavra2: %s\n  Impostor: %s", tema, profissoes[palavra], profissoes[palavra2], nomes[impostor]);
            break;
        case 4:
            palavra = aleatorio(0, 42);
            palavra2 = aleatorio(0, 42);
            rodada(tema, palavra, palavra2, impostor);
            //iprintf("\n\n  Tema: %d\n  Palavra: %s\n  Palavra2: %s\n  Impostor: %s", tema, animais[palavra], animais[palavra2], nomes[impostor]);
            break;
    }

    int primeiro = aleatorio(0, totalNomes);

    while(pmMainLoop()){
        consoleSelect(&topScreen);
        consoleClear();

        scanKeys();
        int key = keysDown();
        int key2 = keysHeld();

        if(key & KEY_START){
            return;
        }

        iprintf("\n\n\n\n\n\n\n\n\n  Pressione Start para voltar\n            ao menu");
        iprintf("\n\n   Segure Y para revelar o\n            impostor");

        consoleSelect(&bottomScreen);
        consoleClear();

        iprintf("\n\n\n\n\n\n\n\n\n\n\n    %s vai comecar!", nomes[primeiro]);

        if(key2 & KEY_Y){
            iprintf("\n\n    Impostor: %s", nomes[impostor]);
        }

        swiWaitForVBlank();
    }
}
void secreto(){
    int palavra[totalNomes];

    int tema = aleatorio(0, NUM_TEMAS);

    int passo = 1;

    for(int i = 0; i < totalNomes; i++){
        if(i == 0){
            switch(tema){
                case 0:
                    palavra[i] = aleatorio(0, 36);
                    break;
                case 1:
                    palavra[i] = aleatorio(0, 19);
                    break;
                case 2:
                    palavra[i] = aleatorio(0, 14);
                    break;
                case 3:
                    palavra[i] = aleatorio(0, 19);
                    break;
                case 4:
                    palavra[i] = aleatorio(0, 42);
                    break;
            }
        }
        else{
            switch(tema){
                case 0:
                    palavra[i] = aleatorio(0, 36);
                    break;
                case 1:
                    palavra[i] = aleatorio(0, 19);
                    break;
                case 2:
                    palavra[i] = aleatorio(0, 14);
                    break;
                case 3:
                    palavra[i] = aleatorio(0, 19);
                    break;
                case 4:
                    palavra[i] = aleatorio(0, 42);
                    break;
            }
            while(palavra[i] == palavra[i - passo]){
                switch(tema){
                    case 0:
                        palavra[i] = aleatorio(0, 36);
                        break;
                    case 1:
                        palavra[i] = aleatorio(0, 19);
                        break;
                    case 2:
                        palavra[i] = aleatorio(0, 14);
                        break;
                    case 3:
                        palavra[i] = aleatorio(0, 19);
                        break;
                    case 4:
                        palavra[i] = aleatorio(0, 42);
                        break;
                }
                if(passo < totalNomes) passo++;
            }
        }
    }
    consoleSelect(&bottomScreen);


    for(int i = 0; i < totalNomes; i++){
        while(pmMainLoop()){
            consoleSelect(&topScreen);
            consoleClear();

            iprintf("\n\n\n\n\n\n\n\n     Para ver sua palavra\n     segure Y");
            iprintf("\n\n  Para prosseguir pressione A");

            consoleSelect(&bottomScreen);
            consoleClear();

            scanKeys();
            int key = keysHeld();
            int key2 = keysDown();

            iprintf("\n\n          %s\n\n\n\n\n\n\n\n", nomes[i]);

            if(key & KEY_Y){
                switch(tema){
                    case 0:
                        iprintf("          %s", comidas[palavra[i]]);
                        break;
                    case 1:
                        iprintf("          %s", paises[palavra[i]]);
                        break;
                    case 2:
                        iprintf("          %s", emocoes[palavra[i]]);
                        break;
                    case 3:
                        iprintf("          %s", profissoes[palavra[i]]);
                        break;
                    case 4:
                        iprintf("          %s", animais[palavra[i]]);
                        break;
                }
            }
            
            if(key2 & KEY_A) break;
                
            swiWaitForVBlank();
        }   
    }
    int primeiro = aleatorio(0, totalNomes);

    while(pmMainLoop()){
        scanKeys();
        int key2 = keysHeld();
        int key = keysDown();

        consoleSelect(&topScreen);
        consoleClear();

        iprintf("\n\n\n\n\n\n\n\n\n  Pressione Start para voltar\n            ao menu");
        iprintf("\n\n   Segure Y para revelar o\n            impostor");

        consoleSelect(&bottomScreen);
        consoleClear();

        iprintf("\n\n\n\n\n\n\n\n\n\n\n    %s vai comecar!", nomes[primeiro]);

        if(key2 & KEY_Y){
            iprintf("\n\n    Impostor: TODO MUNDO!");
        }
        if(key & KEY_START){
            return;
        }

        swiWaitForVBlank();
    }    
}