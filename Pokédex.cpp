// Beatriz Vaz, Felipe Loschi, Felipe Reis, João Gabriel Chereze, Pedro Henrique Dias
#include <iostream>
#include <iomanip>
#include <list>
#include <climits> // Grafos.
#include <locale.h> // Possibilitar a acentuação.
#include <windows.h> // Para conseguir alterar a cor do terminal.
#include <cmath> // Algoritmos geométricos.
#include <vector> // Algoritmos geométricos.
#include <stack> // Menor caminho Djikstra.

using namespace std;

string tipo1[18] =
{
	"Aço", "Água", "Dragão", "Elétrico", "Fada", "Fantasma", "Fogo", "Gelo", "Grama",
	"Inseto", "Lutador", "Normal", "Noturno", "Pedra", "Psíquico", "Terrestre", "Venenoso", "Voador"
};

string tipo2[19] =
{
	"Aço", "Água", "Dragão", "Elétrico", "Fada", "Fantasma", "Fogo", "Gelo", "Grama",
	"Inseto", "Lutador", "Normal", "Noturno", "Pedra", "Psíquico", "Terrestre", "Venenoso", "Voador", "Null"
};

struct ponto
{
	int x, y;
};

struct pokemon
{
	string nome;
	int id, tipo1, tipo2;
	ponto posicao; // Coordenadas
};

struct treenode
{
	pokemon dados;
	treenode *left;
	treenode *right;
	int height; // AVL.
};

struct Estrada
{
	int origem, destino, peso;
};

struct cidades
{
	list<Estrada> vizinhos;
	int codigo;	 // codigo da cidade
	bool centro; // possui um centro poke ou nao
	string nome; // nome da cidade
};

void entrada_grafo(cidades cidade[], int num_vertices, int arestas, bool orientado)
{
	int origem, destino, peso;

	for (int i = 0; i < num_vertices; i++)
	{
		cout << "Agora preciso que informe o nome da cidade: " << endl;
		getline(cin >> ws, cidade[i].nome);
		cout << "Agora preciso que informe o código da cidade: " << endl;
		cin >> cidade[i].codigo;
		cout << "Para verificação de centros pokémon na cidade, digite '0' para 'não' e '1' para 'sim' : " << endl;
		cin >> cidade[i].centro;
		cout << endl;
	}

	cout << endl;

	// inserindo os dados do grafo

	// Instrução para entrada dos caminhos do grafo
	cout << "Agora vamos inserir os caminhos entre as cidades." << endl;
	cout << "Para cada caminho, escreva o ponto de origem, o ponto de chegada e a distância entre as cidades." << endl;
	cout << "Exemplo de entrada: '0 1 10' indica um caminho da cidade 0 para a cidade 1 com distância 10." << endl;
	cout << endl;

	for (int i = 0; i < arestas; i++)
	{
		cin >> origem >> destino >> peso;

		cidade[origem].vizinhos.push_back( {origem, destino, peso});

		if (orientado == 0)
		{
			cidade[destino].vizinhos.push_back( {destino, origem, peso});
		}
	}
}

// Função para mostrar as cidades e seus vizinhos.
void print_cidades(cidades cidade[], int numVertices)
{
	list<Estrada>::iterator p;
	cout << "Lista das cidades cadastradas: " << endl;
	cout << endl;
	cout << "| " << setw(2) << "Nome(s) " << setw(15) << " | " << setw(6) << right << setw(10) << "Código" << setw(7) << " | " << setw(15) << " Centro Pokémon" << right << setw(43) << " | Vizinhos - Origem, destino e distância" << endl;
	for (int i = 0; i < numVertices; i++)
	{
		cout << "| ";
		cout << setw(19) << left << setw(20) << left << cidade[i].nome << left << " | ";
		cout << left << setw(8) << right << cidade[i].codigo << right << setw(9) << " | ";
		if (!cidade[i].centro)
			cout << "Não possui centro |";
		else
			cout << right << setw(15) << "Possui centro" << right << setw(4) << " |";

		// Verifica se há vizinhos para esta cidade
		if (!cidade[i].vizinhos.empty())
		{
			cout << " ";
			for (p = cidade[i].vizinhos.begin(); p != cidade[i].vizinhos.end(); p++)
			{
				cout << p->origem << " -> " << p->destino << "(" << p->peso << ")" << " ";
			}
		}
		cout << endl;
	}
}

void shortest_path_dijkstra(cidades cidades[], int vertices, int cod_atual, int &v) {
    bool intree[vertices];
    int distance[vertices], parent[vertices];
    list<Estrada>::iterator p;

    for (int u = 0; u < vertices; u++) {
        intree[u] = false;
        distance[u] = INT_MAX;
        parent[u] = -1;
    }
    distance[cod_atual] = 0;
    v = cod_atual;
    while (!intree[v]) {
        intree[v] = true;
        for (p = cidades[v].vizinhos.begin(); p != cidades[v].vizinhos.end(); p++) {
            int dest = p->destino;
            int weight = p->peso;
            if (distance[dest] > distance[v] + weight) {
                distance[dest] = distance[v] + weight;
                parent[dest] = v;
            }
        }
        v = 0;
        int dist = INT_MAX;
        for (int u = 0; u < vertices; u++) {
            if (!intree[u] && dist > distance[u]) {
                dist = distance[u];
                v = u;
            }
        }
    }
    v = 0;
    int dist = INT_MAX;
    for (int u = 0; u < vertices; u++) {
        if (intree[u] && dist > distance[u] && cidades[u].centro) {
            dist = distance[u];
            v = u;
        }
    }
    // Mostrar menor caminho: 
    cout << "Menor caminho: ";
    stack<int> caminho;
    int atual = v;
    while(atual != cod_atual) {
        caminho.push(atual);
        atual = parent[atual];
    }
    cout << cidades[cod_atual].nome;
    while(!caminho.empty()) {
        cout << " -> " << cidades[caminho.top()].nome;
        caminho.pop();
    }
    cout << endl;
}

// Função que busca a cidade mais próxima com um centro pokémon com auxílio do algoritmo de Dijkstra:

void busca_centro(cidades cidade[], int num_vertices)
{
	int pos = 0, cod_atual, end;
	cout << "Escreva o código da cidade em que você está: ";
	cin >> cod_atual;
	for (int i = 0; i < num_vertices; i++)
	{

		if (cod_atual == cidade[i].codigo)
		{
			pos = i;
			if (cidade[pos].centro)
			{
				cout << "Sua cidade já tem um centro pokémon!" << endl;
				pos = -1;
				break;
			}
		}
	}
	if (pos != -1)
	{

		shortest_path_dijkstra(cidade, num_vertices, pos, end);
		cout << "A cidade mais próxima com um centro Pokémon é: " << cidade[end].nome << endl;
	}
}

typedef treenode *treenodeptr;

// Função que auxilia para mostrar os tipos quando os dados dos pokémons forem inseridos:

void imprime_tipos() {
    cout << "Tipos de Pokémon:";

    for (int i = 0; i < 19; i++) {
        cout << "(" << i << ") " << tipo2[i];
        if (i < 18) {
            cout << ", ";
        } else {
            cout << endl;
        }
    }
}

// Função de inserção dos dados de cada pokémon que será inserido na árvore:

void dados_pokemon(pokemon &poke)
{
	cout << "Escreva o nome, o tipo1, o tipo2, o id, e as coordenadas x e y do seu Pokémon: " << endl;
	cout << "Nome : ";
	getline(cin >> ws, poke.nome);
	imprime_tipos();
	cout << "Tipo 1: ";
	cin >> poke.tipo1;
	cout << "Tipo 2: ";
	cin >> poke.tipo2;
	cout << "ID: ";
	cin >> poke.id;
	cout << "(X, Y) = ";
    cin >> poke.posicao.x >> poke.posicao.y;
}

// Função auxiliar que compara as strings dos nomes para futuras inserções na árvore por nomes:

int compara_nome_pokemon(string &nome, pokemon &poke)
{
	if (nome == poke.nome)
	{
		return 0;
	}
	else if (nome < poke.nome)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

// Função auxiliar que compara os inteiros dos tipos para futuras inserções na árvore por tipos:

int compara_tipo_pokemon(int tipo1, pokemon poke)
{
	if (tipo1 == poke.tipo1)
	{
		return 0;
	}
	else if (tipo1 < poke.tipo1)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

// Função que insere os pokémons por tipo:

void pokeInsert_por_tipo(treenodeptr &p, pokemon poke_tipo)
{
	if (p == NULL)
	{
		p = new treenode;
		p->dados = poke_tipo;
		p->left = NULL;
		p->right = NULL;
	}
	else
	{
		int compare = compara_tipo_pokemon(poke_tipo.tipo1, p->dados);
		if (compare < 0)
		{
			pokeInsert_por_tipo(p->left, poke_tipo);
		}
		else
		{
			pokeInsert_por_tipo(p->right, poke_tipo);
		}
	}
}

// Função que insere os pokémons por nome:

void pokeInsert_por_nome(treenodeptr &p, pokemon poke_nome)
{
	if (p == NULL)
	{
		p = new treenode;
		p->dados = poke_nome;
		p->left = NULL;
		p->right = NULL;
		cout << poke_nome.nome << " inserido com sucesso!" << endl;
	}
	else
	{
		int compare = compara_nome_pokemon(poke_nome.nome, p->dados);
		if (compare == -1)
		{
			pokeInsert_por_nome(p->left, poke_nome);
		}
		else
		{
			pokeInsert_por_nome(p->right, poke_nome);
		}
	}
}

// Função que é responsável pela busca dos pokémons na árvore.

treenodeptr pokeSearch(treenodeptr p, string poke)
{
	if (p == NULL)
	{
		return NULL;
	}
	else
	{
		cout << "Comparando: " << poke << " com " << p->dados.nome << endl;
		if (poke == p->dados.nome) // Elemento encontrado.
		{
			return p;
		}
		else if (poke < p->dados.nome) // Procura na subárvore esquerda.
		{
			return pokeSearch(p->left, poke);
		}
		else // Procura na subárvore direita.
		{
			return pokeSearch(p->right, poke);
		}
	}
}

// Função alivia main:

void pesquisarPokemon(treenodeptr arvore)
{
	string op;
	cout << "Digite o nome do pokémon que deseja pesquisar: ";
	getline(cin >> ws, op);
	treenodeptr encontrado = pokeSearch(arvore, op);
	if (encontrado != NULL)
	{
		cout << "Encontrado" << endl;
	}
	else
	{
		cout << "Não encontrado" << endl;
	}
}

// Função auxiliar para remoção.

treenodeptr tMenor(treenodeptr &p)
{
	treenodeptr t;
	t = p;
	if (t->left == NULL)
	{
		// Encontrou o menor valor
		p = p->right;
		return t;
	}
	else // Continua procurando na esquerda
		return tMenor(p->left);
}

// Função que remove pokémons da árvore pelo seu nome:

bool pokeRemove(treenodeptr &p, string nome)
{
	treenodeptr pk;

	if (p == NULL)
		return false;

	if (compara_nome_pokemon(nome, p->dados) == 0)
	{
		pk = p;

		if (p->left == NULL)
			p = p->right;

		else
		{
			if (p->right == NULL)
				p = p->left;

			else
			{
				pk = tMenor(p->right);
				p->dados = pk->dados;
			}
		}

		delete pk;
		return true;
	}

	else
	{
		if (compara_nome_pokemon(nome, p->dados) == -1)
			return pokeRemove(p->left, nome);
		else
			return pokeRemove(p->right, nome);
	}
}

// Função alivia main:

void removerPokemon(treenodeptr &arvore)
{
	string a_remover;
	bool removido;
	cout << "Digite o nome do pokémon que deseja remover: ";
	getline(cin >> ws, a_remover);
	removido = pokeRemove(arvore, a_remover);
	if (removido)
	{
		cout << "Removido" << endl;
	}
	else
	{
		cout << "Não encontrado para remoção" << endl;
	}
}

// Função que ordena os pokémons em ordem alfabética de nome:

void inOrder_nome(treenodeptr p)
{
	static bool hp = false; // Para conseguir mostrar só uma vez os dados.
	if (p != NULL)
	{
		inOrder_nome(p->left);
		if (!hp)
		{
			cout << "Mostrando os dados dos pokémons em ordem crescente de nome: " << endl;
			cout << endl;
			cout << "Nome            | Tipo1      | Tipo2      | ID    | X     | Y" << endl;
			hp = true;
		}
		cout << left << setw(15) << p->dados.nome
			 << " | " << left << setw(10) << tipo1[p->dados.tipo1]
			 << " | " << left << setw(10) << tipo2[p->dados.tipo2]
			 << " | " << left << setw(5) << p->dados.id
			 << " | " << left << setw(5) << p->dados.posicao.x
			 << " | " << left << setw(5) << p->dados.posicao.y << endl;
		inOrder_nome(p->right);
	}
}

// Função que ordena os pokémons por tipo:

void inOrder_tipo(treenodeptr p)
{
	static bool hp = false;
	if (p != NULL)
	{
		inOrder_tipo(p->left);
		if (!hp)
		{
			cout << "Mostrando os dados dos pokémons em ordem crescente de tipo: " << endl;
			cout << endl;
			cout << "Nome            | Tipo1      | Tipo2      | ID    | X     | Y" << endl;
			hp = true;
		}
		cout << left << setw(15) << p->dados.nome
			 << " | " << left << setw(10) << tipo1[p->dados.tipo1]
			 << " | " << left << setw(10) << tipo2[p->dados.tipo2]
			 << " | " << left << setw(5) << p->dados.id
			 << " | " << left << setw(5) << p->dados.posicao.x
			 << " | " << left << setw(5) << p->dados.posicao.y << endl;
		inOrder_tipo(p->right);
	}
}

// Função que conta os pokémons por tipo:

void conta_tipo(treenodeptr p, int tipo, int &i)
{

	if (p != NULL)
	{
		// cout << "Comparando: " << tipo << " com " << p->dados.tipo1 << endl;
		if (tipo == p->dados.tipo1 || tipo == p->dados.tipo2) // Elemento encontrado.
		{
			i++;
		}
		conta_tipo(p->left, tipo, i);
		conta_tipo(p->right, tipo, i);
	}
}

// Função auxiliar da conta_tipo:

void imprime_poke_tipo(treenodeptr p)
{

	int escolha_tipo = 0, i = 0;
	cout << "Para mostrar quantos pokémons por tipo, digite os seguintes números para acessar : " << endl;
	cout << endl;
	do
	{
		cout << "0 - Aço"
			 << endl
			 << "1 - Água"
			 << endl
			 << "2 - Dragão"
			 << endl
			 << "3 - Elétrico"
			 << endl
			 << "4 - Fada"
			 << endl
			 << "5 - Fantasma"
			 << endl
			 << "6 - Fogo"
			 << endl
			 << "7 - Gelo"
			 << endl
			 << "8 - Grama"
			 << endl
			 << "9 - Inseto"
			 << endl
			 << "10 - Lutador"
			 << endl
			 << "11 - Normal"
			 << endl
			 << "12 - Noturno"
			 << endl
			 << "13 - Pedra"
			 << endl
			 << "14 - Psíquico"
			 << endl
			 << "15 - Terrestre"
			 << endl
			 << "16 - Venenoso"
			 << endl
			 << "17 - Voador"
			 << endl
			 << "Caso queira sair, insira um número fora do intervalo de 0 a 17"
			 << endl;

		cin >> escolha_tipo;
		if (escolha_tipo < 0 || escolha_tipo > 17)
		{
			break;
		}
		conta_tipo(p, escolha_tipo, i);
		cout << endl;
		cout << "Há " << i << " Pokémon(s) do tipo " << tipo1[escolha_tipo] << endl;
		cout << endl;
		i = 0; // Resetar p/ a próxima iteração.
	}
	while (escolha_tipo >= 0 && escolha_tipo <= 17);
	cout << endl;
}

// caso 10:
int orientation(ponto p, ponto q, ponto r)
{
	int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

	if (val == 0)
		return 0;			  // colinear
	return (val > 0) ? 1 : 2; // horario ou anti-horario
}

double calcular_distancia(ponto p1, ponto p2)
{
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;
	double distancia = sqrt(dx * dx + dy * dy);

	return distancia;
}

// Algoritmo importante para montar o fecho convexo:

void gift_wraping(vector<ponto> &points, int n)
{
	double perimetro = 0;

	// Deve haver pelo menos 3 pontos
	if (n < 3)
		return;

	// Inicializando a resposta
	vector<ponto> fecho;

	// Encontra o ponto mais a esquerda
	int l = 0;
	for (int i = 1; i < n; i++)
		if (points[i].x < points[l].x)
			l = i;

	int p = l, q;
	do
	{
		fecho.push_back(points[p]);

		q = (p + 1) % n;
		for (int i = 0; i < n; i++)
		{
			if (orientation(points[p], points[i], points[q]) == 2)
			{
				q = i;
			}
		}

		p = q;

	}
	while (p != l);

	// Imprime o resultado
	for (int i = 0; (i < fecho.size() - 1); i++)
	{
		perimetro += calcular_distancia(fecho[i], fecho[i + 1]);
	}

	perimetro += calcular_distancia(fecho[0], fecho[fecho.size() - 1]);

	cout << "Perimetro: " << perimetro << endl;

	points.clear();
}

// Função que conta quantos pokémons estão dentro do raio de 100 metros:

void contar_pokemons(treenodeptr p, ponto minha_posicao, int &contador, vector<ponto> &pilha)
{
	if (p != NULL)
	{
		if (calcular_distancia(minha_posicao, p->dados.posicao) <= 100)
		{
			contador++;
			pilha.push_back(p->dados.posicao);
		}
		contar_pokemons(p->left, minha_posicao, contador, pilha);
		contar_pokemons(p->right, minha_posicao, contador, pilha);
	}
}

void consultar_pokemons_no_raio(treenodeptr p)
{
	ponto minha_posicao;
	vector<ponto> pilha;
	int contador = 0;

	cout << "Para ter acesso à quantidade de pokémons dentro de um raio de 100 metros, digite suas coordenadas (x e y): " << endl;
	cin >> minha_posicao.x >> minha_posicao.y;

	pilha.push_back(minha_posicao);

	contar_pokemons(p, minha_posicao, contador, pilha);

	cout << contador << " pokémons dentro do raio!" << endl;

	gift_wraping(pilha, int(pilha.size()));
}

// Função alivia main:

void exibe_introducao(int &orientado, int &num_vertices, int &arestas)
{
	// Introdução
	cout << "Olá, treinador. Bem vindo ao sistema de localização de pokémons!" << endl;
	// cout << "\x1b[3mEste texto está em itálico!\x1b[0m" << endl; // Texto em itálico.
	cout << endl;

	// Entrada dos dados do grafo a ser contruido:
	cout << "Escreva o número de cidades e o número de caminhos:" << endl;
	cin >> num_vertices >> arestas;

	// Definindo se o grafo sera orientado ou não:
	cout << "Agora, preciso que escolha se há possibilidade de retorno de uma cidade a outra, para isso: " << endl;
	cout << "Digite '0' para dizer se o caminho será não orientado ou '1' para orientado." << endl;

	do
	{
		cin >> orientado;
		if (orientado == 0 || orientado == 1)
		{
			break;
		}
		cout << "Desculpe, valor inserido fora dos limites declarados!" << endl;
		cout << "Digite '0' para dizer se o caminho será náo orientado ou '1' para orientado." << endl;

	}
	while (orientado != 0 || orientado != 1);
}

// Função alivia main:

void Imprime_menu()
{
	cout << "MENU" << endl
		 << "1-Entrada de dados das cidades"
		 << endl
		 << "2-Imprimir as cidades cadastradas"
		 << endl
		 << "3-Buscar cidade com centro pokémon mais próximo"
		 << endl
		 << "4-Fazer a Inserção dos Pokémons"
		 << endl
		 << "5-Pesquisar os pokémons"
		 << endl
		 << "6-Remover algum pokémon"
		 << endl
		 << "7-Imprimir pokémons cadastrados em ordem alfabética de nome"
		 << endl
		 << "8-Imprimir pokémons cadastrados em ordem alfabética de tipo"
		 << endl
		 << "9-Mostrar quantos pokémons de determinado tipo"
		 << endl
		 << "10-Mostrar quantos pokémons podem ser encontrados dentro de um raio de 100 metros e o "
		 << endl
		 << "perímetro do fecho convexo formado por esses pokémons"
		 << endl
		 << "11-Mostrar a diferença de comparações na busca entre a árvore normal e a balanceada(AVL)"
		 << endl
		 << "Pressione qualquer outro número pra sair"
		 << endl;
}

// AVL:

int height(treenodeptr n)
{
	return n ? n->height : 0;
}

int max(int a, int b)
{
	return (a > b) ? a : b;
}

treenodeptr newNode(pokemon data)
{
	treenodeptr node = new treenode();
	node->dados = data;
	node->left = node->right = NULL;
	node->height = 1; // Novo nó é inicialmente adicionado na folha
	return node;
}

treenodeptr rightRotate(treenodeptr y)
{
	treenodeptr x = y->left;
	treenodeptr T2 = x->right;

	x->right = y;
	y->left = T2;

	y->height = max(height(y->left), height(y->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;

	return x;
}

treenodeptr leftRotate(treenodeptr x)
{
	treenodeptr y = x->right;
	treenodeptr T2 = y->left;

	y->left = x;
	x->right = T2;

	x->height = max(height(x->left), height(x->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;

	return y;
}

int getBalance(treenodeptr n)
{
	return n ? height(n->left) - height(n->right) : 0;
}

treenodeptr pokeInsert_nome_AVL(treenodeptr node, pokemon data)
{
	if (!node)
		return newNode(data);

	if (data.nome < node->dados.nome)
		node->left = pokeInsert_nome_AVL(node->left, data);
	else if (data.nome > node->dados.nome)
		node->right = pokeInsert_nome_AVL(node->right, data);
	else // Nomes iguais não são permitidos na BST
		return node;

	node->height = 1 + max(height(node->left), height(node->right));

	int balance = getBalance(node);

	// Caso Esquerda-Esquerda
	if (balance > 1 && data.nome < node->left->dados.nome)
		return rightRotate(node);

	// Caso Direita-Direita
	if (balance < -1 && data.nome > node->right->dados.nome)
		return leftRotate(node);

	// Caso Esquerda-Direita
	if (balance > 1 && data.nome > node->left->dados.nome)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Caso Direita-Esquerda
	if (balance < -1 && data.nome < node->right->dados.nome)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}

void pesquisarPokemon(treenodeptr p, string nome, int &contador)
{
	if(p == NULL)    // Se o nó for nulo (elemento não encontrado)
	{
		return;
	}
	else if(nome == p->dados.nome)      // Se o elemento for encontrado na raiz
	{
		return;
	}
	else if(nome < p->dados.nome)      // Se o elemento procurado for menor que a informação do nó
	{
		contador++;  // Incrementa o contador
		pesquisarPokemon(p->left, nome, contador);  // Procura na subárvore esquerda
	}
	else      // Se o elemento procurado for maior que a informação do nó
	{
		contador++;  // Incrementa o contador
		pesquisarPokemon(p->right, nome, contador);  // Procura na subárvore direita
	}
}

void comparaAVL(string &nome_poke,treenodeptr &arvore_AVL, treenodeptr &arvore_por_nome)
{
	cout << "Insira o nome do Pokémon que deseja buscar: ";
	cin >> nome_poke;
	// Caso 5: Busca os pokémons na árvore
	int contador;
	contador = 1; // Inicializa o contador com 1
	pesquisarPokemon(arvore_AVL, nome_poke, contador);
	cout << "Número de comparações AVL: " << contador << endl;
	contador = 1;
	pesquisarPokemon(arvore_por_nome, nome_poke, contador);
	cout << "Número de comparações da árvore normal: " << contador << endl;
}

// Função que deleta os nós e destrói uma árvore:

void tDestruir(treenodeptr &p)
{
	if(p != NULL)
	{
		tDestruir(p->left);
		tDestruir(p->right);
		delete p;
	}
	p = NULL;
}


int main()
{
	setlocale(LC_ALL, ""); // Permitindo acentuação no código.
	system("color 74"); // Altera cor do terminal.

	// Declarando variaveis
	int num_vertices, arestas, orientado, menu = 1;
	string nome_poke;
	treenodeptr arvore_por_nome = NULL;
	treenodeptr arvore_por_tipo = NULL;
	treenodeptr arvore_AVL = NULL;
	pokemon novo_pokemon;

	exibe_introducao(orientado, num_vertices, arestas);

	cidades cidade[num_vertices];

	// Codigo de menu simples, usando a variavel menu no loop
	while (menu != 0)
	{
		// Introducao do menu e entrada da variavel
		cout << endl;
		Imprime_menu();
		cin >> menu;
		cout << endl;

		// Usando switch case para o menu
		switch (menu)
		{
			// Caso 1: Entrada do grafo
		case 1:
			// fazendo a inicialização dos dados das cidades (codigo, centro poke e nome)
			entrada_grafo(cidade, num_vertices, arestas, orientado);
			break;
			// Caso 2: Imprime as cidades salvas
		case 2:
			print_cidades(cidade, num_vertices);
			break;
			// Caso 3: Busca a cidade com centro pokémon mais próxima.
		case 3:
			busca_centro(cidade, num_vertices);
			break;
		case 4:
			// Caso 4: Insere os pokémons na árvore.
			dados_pokemon(novo_pokemon);
			pokeInsert_por_nome(arvore_por_nome, novo_pokemon);
			pokeInsert_por_tipo(arvore_por_tipo, novo_pokemon);
			arvore_AVL = pokeInsert_nome_AVL(arvore_AVL, novo_pokemon);
			break;
		case 5:
			// Caso 5: busca os pokémons na árvore.
			pesquisarPokemon(arvore_por_nome);
			break;
		case 6:
			// Caso 6: Remove os pokémons na árvore.
			removerPokemon(arvore_por_nome);
			break;
		case 7:
			// Caso 7: Mostra os pokémons em ordem de nome.
			inOrder_nome(arvore_por_nome);
			break;
		case 8:
			// Caso 8: Mostra os pokémons em ordem de tipo.
			inOrder_tipo(arvore_por_tipo);
			break;
		case 9:
			// Caso 9: Conta quantos pokémons de determinado tipo:
			imprime_poke_tipo(arvore_por_tipo);
			break;
		case 10:
			// caso 10: Mostra quantos pokémons podem ser encontrados dentro de um raio de 100 metros.
			consultar_pokemons_no_raio(arvore_por_nome);
			break;
		case 11:
			// Caso 11: Mostra a comparação entre as árvores.
			comparaAVL(nome_poke, arvore_AVL, arvore_por_nome);
			break;
		default: // Caso receba outro valor: sai do loop.
			menu = 0;
			break;
		}
	}
	
	// Destruindo as árvores:

	tDestruir(arvore_por_nome);
	tDestruir(arvore_por_tipo);
	tDestruir(arvore_AVL);

	return 0;
}

// Para o caso 3, escolher o código 134 no caso teste para melhor visualização, para os outros, ou as cidades são vizinhas,
// ou já tem um centro pokémon.

/*

Caso teste com inserção de cidades e pokémons, sem as funcionalidades adicionais a partir do caso 5.

10 12
0
1
Pallet
251
0
Lumiose
72
1
Tapu village
302
1
Lavender Town
134
0
Verdanturf Town
192
0
Vaniville Town
16
0
Aether Paradise
293
0
Opalucid City
150
1
Suntouched City
280
1
Celestic Town
205
1

0 4 200
0 3 20
0 8 300
0 9 400
1 5 40
2 6 30
4 2 150
4 7 220
4 8 250
7 1 175
8 1 600
8 9 320

2

3
16
3
302

4
Lucario
10
0
448
72 91
4
Decidueye
8
5
724
65 36
4
Zapdos
3
17
145
120 52
4
Greninja
1
12
658
47 158
4
Blaziken
6
10
257
4 190
4
Mew
14
18
151
169 74
4
Eevee
11
18
133
22 77
4
MewTwo
14
18
680
75 39
4
Vulpix Alola
7
18
299
156 56
4
Psyduck
1
18
320
165 89
4
Scizor
9
0
212
82 61
4
Pinsir
9
18
127
45 73
4
Snorlax
11
18
143
103 55
4
Onix
13
7
95
17 44
4
Tyranitar
13
12
248
59 22
4
Dragonite
2
17
149
124 97
4
Salamence
2
17
373
85 43
4
Bulbasaur
8
16
1
53 92
4
Sceptile
8
18
254
39 61
4
Umbreon
12
18
197
109 30
4
Absol
12
18
359
42 68
4
Golem
13
7
76
102 118
4
Flygon
15
7
330
25 84
4
Gardevoir
14
5
282
121 37
4
Togekiss
14
17
468
58 112
4
Ninetales Alola
7
15
505
45 99
4
Solosis
14
18
577
190 26
4
Minun
3
18
312
115 150

*/
