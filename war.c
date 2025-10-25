// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO - NÍVEL MESTRE
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
#define NUM_TERRITORIOS 5
#define NUM_MISSOES 2
#define TAM_STRING 50

// --- Estrutura de Dados ---
typedef struct {
    char nome[TAM_STRING];
    char corExercito[TAM_STRING];
    int numTropas;
} Territorio;

// --- Protótipos das Funções ---

// Funções de setup e gerenciamento de memória
Territorio* alocarMapa();
void inicializarTerritorios(Territorio *mapa);
void liberarMemoria(Territorio *mapa);

// Funções de interface com o usuário
void exibirMenuPrincipal();
void exibirMapa(const Territorio *mapa);
void exibirMissao(int idMissao);

// Funções de lógica principal do jogo
void faseDeAtaque(Territorio *mapa, const char *corJogador);
void simularAtaque(Territorio *mapa, int origem, int destino, const char *corJogador);
int sortearMissao();
int verificarVitoria(const Territorio *mapa, int idMissao, const char *corJogador);

// Função utilitária
void limparBufferEntrada();

// --- Função Principal (main) ---
int main() {
    // 1. Configuração Inicial (Setup)
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));
    
    Territorio *mundoWar = alocarMapa();
    if (mundoWar == NULL) {
        printf("Erro ao alocar memória para o mapa!\n");
        return 1;
    }
    
    inicializarTerritorios(mundoWar);
    
    char corJogador[TAM_STRING] = "Azul";
    int missaoJogador = sortearMissao();
    int opcao;
    int jogoAtivo = 1;
    
    printf("\n═══════════════════════════════════════════════════════════\n");
    printf("       🎮 BEM-VINDO AO WAR ESTRUTURADO - NÍVEL MESTRE 🎮\n");
    printf("═══════════════════════════════════════════════════════════\n");
    printf("\nVocê é o comandante do exército %s!\n", corJogador);
    exibirMissao(missaoJogador);
    printf("\nPressione ENTER para começar...");
    getchar();
    
    // 2. Laço Principal do Jogo (Game Loop)
    do {
        system("clear || cls"); // Limpa a tela (funciona em Linux/Mac e Windows)
        
        exibirMapa(mundoWar);
        printf("\n");
        exibirMissao(missaoJogador);
        printf("\n");
        exibirMenuPrincipal();
        
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();
        
        switch (opcao) {
            case 1:
                faseDeAtaque(mundoWar, corJogador);
                break;
                
            case 2:
                if (verificarVitoria(mundoWar, missaoJogador, corJogador)) {
                    printf("\n🎉🎉🎉 PARABÉNS! VOCÊ COMPLETOU SUA MISSÃO! 🎉🎉🎉\n");
                    printf("Você é o grande vencedor do WAR!\n");
                    jogoAtivo = 0;
                } else {
                    printf("\n⚠️  Você ainda não completou sua missão.\n");
                    printf("Continue lutando pela vitória!\n");
                }
                break;
                
            case 0:
                printf("\n👋 Encerrando o jogo... Até a próxima batalha!\n");
                jogoAtivo = 0;
                break;
                
            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
                break;
        }
        
        if (jogoAtivo) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }
        
    } while (jogoAtivo);
    
    // 3. Limpeza
    liberarMemoria(mundoWar);
    
    return 0;
}

// --- Implementação das Funções ---

// Aloca dinamicamente a memória para o vetor de territórios
Territorio* alocarMapa() {
    Territorio *mapa = (Territorio*) calloc(NUM_TERRITORIOS, sizeof(Territorio));
    return mapa;
}

// Preenche os dados iniciais de cada território no mapa
void inicializarTerritorios(Territorio *mapa) {
    // Território 0
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].corExercito, "Azul");
    mapa[0].numTropas = 5;
    
    // Território 1
    strcpy(mapa[1].nome, "Argentina");
    strcpy(mapa[1].corExercito, "Verde");
    mapa[1].numTropas = 3;
    
    // Território 2
    strcpy(mapa[2].nome, "Chile");
    strcpy(mapa[2].corExercito, "Vermelho");
    mapa[2].numTropas = 4;
    
    // Território 3
    strcpy(mapa[3].nome, "Peru");
    strcpy(mapa[3].corExercito, "Verde");
    mapa[3].numTropas = 2;
    
    // Território 4
    strcpy(mapa[4].nome, "Uruguai");
    strcpy(mapa[4].corExercito, "Amarelo");
    mapa[4].numTropas = 3;
}

// Libera a memória previamente alocada para o mapa
void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

// Imprime na tela o menu de ações disponíveis
void exibirMenuPrincipal() {
    printf("╔═══════════════════════════════════════╗\n");
    printf("║         MENU PRINCIPAL                ║\n");
    printf("╠═══════════════════════════════════════╣\n");
    printf("║  1 - ⚔️  Atacar Território            ║\n");
    printf("║  2 - 🎯 Verificar Missão              ║\n");
    printf("║  0 - 🚪 Sair do Jogo                  ║\n");
    printf("╚═══════════════════════════════════════╝\n");
}

// Mostra o estado atual de todos os territórios no mapa
void exibirMapa(const Territorio *mapa) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                    🗺️  MAPA DO MUNDO WAR 🗺️                      ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════╣\n");
    printf("║  ID  │  Território       │  Exército      │  Tropas             ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════╣\n");
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("║  %-3d │  %-16s │  %-13s │  %-3d                ║\n",
               i + 1,
               mapa[i].nome,
               mapa[i].corExercito,
               mapa[i].numTropas);
    }
    
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");
}

// Exibe a descrição da missão atual do jogador
void exibirMissao(int idMissao) {
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                    🎯 SUA MISSÃO SECRETA 🎯                       ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════╣\n");
    
    if (idMissao == 0) {
        printf("║  Destruir completamente o exército VERDE!                        ║\n");
        printf("║  (Conquiste todos os territórios verdes)                         ║\n");
    } else if (idMissao == 1) {
        printf("║  Conquistar pelo menos 3 territórios para seu exército!          ║\n");
        printf("║  (Domine 3 ou mais territórios com sua cor)                      ║\n");
    }
    
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");
}

// Gerencia a fase de ataque
void faseDeAtaque(Territorio *mapa, const char *corJogador) {
    int origem, destino;
    
    printf("\n⚔️  FASE DE ATAQUE ⚔️\n");
    printf("════════════════════\n");
    
    printf("\nEscolha o território de ORIGEM (atacante): ");
    scanf("%d", &origem);
    limparBufferEntrada();
    
    printf("Escolha o território de DESTINO (defensor): ");
    scanf("%d", &destino);
    limparBufferEntrada();
    
    // Ajusta para índices do array (0 a 4)
    origem--;
    destino--;
    
    simularAtaque(mapa, origem, destino, corJogador);
}

// Executa a lógica de uma batalha entre dois territórios
void simularAtaque(Territorio *mapa, int origem, int destino, const char *corJogador) {
    // Validações
    if (origem < 0 || origem >= NUM_TERRITORIOS || destino < 0 || destino >= NUM_TERRITORIOS) {
        printf("\n❌ Erro: Territórios inválidos!\n");
        return;
    }
    
    if (origem == destino) {
        printf("\n❌ Erro: Você não pode atacar o mesmo território!\n");
        return;
    }
    
    if (strcmp(mapa[origem].corExercito, corJogador) != 0) {
        printf("\n❌ Erro: Você só pode atacar com territórios do seu exército (%s)!\n", corJogador);
        return;
    }
    
    if (strcmp(mapa[destino].corExercito, corJogador) == 0) {
        printf("\n❌ Erro: Você não pode atacar seu próprio território!\n");
        return;
    }
    
    if (mapa[origem].numTropas < 2) {
        printf("\n❌ Erro: Você precisa de pelo menos 2 tropas para atacar!\n");
        return;
    }
    
    // Simulação da batalha
    printf("\n⚔️  BATALHA: %s (%s) vs %s (%s)\n",
           mapa[origem].nome, mapa[origem].corExercito,
           mapa[destino].nome, mapa[destino].corExercito);
    
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;
    
    printf("\n🎲 Dados lançados:\n");
    printf("   Atacante (%s): %d\n", mapa[origem].nome, dadoAtacante);
    printf("   Defensor (%s): %d\n", mapa[destino].nome, dadoDefensor);
    
    // Resultado da batalha
    if (dadoAtacante > dadoDefensor) {
        mapa[destino].numTropas--;
        printf("\n✅ O atacante venceu! O defensor perdeu 1 tropa.\n");
        
        // Verifica se o território foi conquistado
        if (mapa[destino].numTropas == 0) {
            strcpy(mapa[destino].corExercito, corJogador);
            mapa[destino].numTropas = 1;
            mapa[origem].numTropas--;
            
            printf("\n🎉 TERRITÓRIO CONQUISTADO! %s agora pertence ao exército %s!\n",
                   mapa[destino].nome, corJogador);
        }
    } else {
        mapa[origem].numTropas--;
        printf("\n❌ O defensor resistiu! O atacante perdeu 1 tropa.\n");
    }
    
    printf("\n📊 Estado após a batalha:\n");
    printf("   %s: %d tropas\n", mapa[origem].nome, mapa[origem].numTropas);
    printf("   %s: %d tropas (%s)\n",
           mapa[destino].nome, mapa[destino].numTropas, mapa[destino].corExercito);
}

// Sorteia e retorna um ID de missão aleatório
int sortearMissao() {
    return rand() % NUM_MISSOES;
}

// Verifica se o jogador cumpriu os requisitos de sua missão atual
int verificarVitoria(const Territorio *mapa, int idMissao, const char *corJogador) {
    if (idMissao == 0) {
        // Missão: Destruir o exército Verde
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].corExercito, "Verde") == 0) {
                return 0; // Ainda existe território verde
            }
        }
        return 1; // Todos os territórios verdes foram conquistados
        
    } else if (idMissao == 1) {
        // Missão: Conquistar 3 territórios
        int territoriosConquistados = 0;
        
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].corExercito, corJogador) == 0) {
                territoriosConquistados++;
            }
        }
        
        return territoriosConquistados >= 3;
    }
    
    return 0;
}

// Função utilitária para limpar o buffer de entrada
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
