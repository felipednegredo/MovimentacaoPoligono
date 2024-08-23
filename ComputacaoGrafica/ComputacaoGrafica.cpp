#include <iostream>
#include <GL/freeglut.h>
#include <vector>
using vertice = std::pair<double, double>;
using lista_vertices = std::vector<vertice>;
using aresta = std::pair<int, int>;
using lista_arestas = std::vector<aresta>;

// Estrutura de dados para representar um poligono
struct Poligono {
	double tamanhoLado;
	int numLados;
	vertice posicao;
	vertice escala;
	double rotacao;
	lista_vertices vertices;
	lista_arestas arestas;
};

// Funcao para criar um poligono regular
Poligono criar_poligono(double posicao_x, double posicao_y, double tamanho_lado, int num_lados);
void desenhar(Poligono poligono);
void movimentar(Poligono& poligono, double distancia, double angulo);
void escalar(Poligono& poligono, double escala_x, double escala_y);
void rotacionar(Poligono& poligono, double angulo);
void display();
void redraw(int value);
void keyboard(unsigned char key, int x, int y);
void keyboard_special(int key, int x, int y);

Poligono pentagono;

int delay = 10;

int main(int argc, char** argv) {

	// Cria figura com tamanho X e Y, com tamanho de lado Z e W lados
	pentagono = criar_poligono(128, 128, 30, 5);

	glutInit(&argc, argv);

	glutInitWindowSize(512, 512);

	glutCreateWindow("Movimentação escalar, rotacioanar e movimentar");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glOrtho(0, 256, 0, 256, -1, 1);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_special);
	glutTimerFunc(10, redraw, 0);

	glutMainLoop();

	return 0;
}


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	desenhar(pentagono);

	glFlush();
}

void keyboard(unsigned char key, int x, int y) {
	std::cout << key;
	switch (key) {
	case 27:
		exit(0);
		break;
	case ' ':
		escalar(pentagono, 1.1, 1.1);
		break;
	}
}

void keyboard_special(int key, int x, int y) {
	std::string key_string;
	switch (key) {
	case GLUT_KEY_DOWN:
		key_string = "DOWN";
		movimentar(pentagono, 10, (270 / 180.0) * 3.1416);
		break;

	case GLUT_KEY_UP:
		key_string = "UP";
		movimentar(pentagono, 10, (90 / 180.0) * 3.1416);
		break;

	case GLUT_KEY_RIGHT:
		key_string = "RIGHT";
		movimentar(pentagono, 10, (0 / 180.0) * 3.1416);
		break;

	case GLUT_KEY_LEFT:
		key_string = "LEFT";
		movimentar(pentagono, 10, (180 / 180.0) * 3.1416);
		break;

	case GLUT_KEY_F1:
		key_string = "F1";
		rotacionar(pentagono, (10 / 180.0) * 3.1416);
		break;

	case GLUT_KEY_F2:
		key_string = "F2";
		rotacionar(pentagono, (-10 / 180.0) * 3.1416);
		break;

	case GLUT_KEY_F3:
		key_string = "F3";
		escalar(pentagono, 1.1, 1.1);
		break;

	case GLUT_KEY_F4:
		key_string = "F4";
		escalar(pentagono, 0.9, 0.9);
		break;

	}
	std::cout << key_string << std::endl;
}

void redraw(int value) {
	glutPostRedisplay();
	glutTimerFunc(delay, redraw, 0);
}

Poligono criar_poligono(double posicao_x, double posicao_y, double tamanho_lado, int num_lados) {
	Poligono novo_poligono;
	novo_poligono.numLados = num_lados;
	novo_poligono.posicao.first = posicao_x;
	novo_poligono.posicao.second = posicao_y;
	novo_poligono.tamanhoLado = tamanho_lado;

	novo_poligono.escala.first = 1;
	novo_poligono.escala.second = 1;

	novo_poligono.rotacao = 0;

	float angulo = 0;
	float passo_angulo = ((360 / float(num_lados)) * 3.1415926536) / 180.0;

	float apothem = tamanho_lado / (2 * tan(3.1416 / float(num_lados)));
	posicao_x -= tamanho_lado / 2.0;
	posicao_y -= apothem;

	novo_poligono.vertices.push_back(vertice(posicao_x, posicao_y));
	std::cout << "Vertices:\n";
	std::cout << 0 << " - " << posicao_x << " - " << posicao_y << "\n";
	for (int i = 1; i < num_lados; i++) {
		posicao_x = posicao_x + tamanho_lado * cos(angulo);
		posicao_y = posicao_y + tamanho_lado * sin(angulo);
		novo_poligono.vertices.push_back(vertice(posicao_x, posicao_y));
		std::cout << i << " - " << posicao_x << " - " << posicao_y << "\n";
		angulo += passo_angulo;
	}

	std::cout << "Arestas:\n";
	for (int i = 0; i < num_lados; i++) {
		novo_poligono.arestas.push_back(aresta(i, (i + 1) % num_lados));
		std::cout << i << " - " << (i + 1) % num_lados << "\n";
	}


	return novo_poligono;
}

void movimentar(Poligono& poligono, double distancia, double angulo) {
	poligono.posicao.first += distancia * cos(angulo);
	poligono.posicao.second += distancia * sin(angulo);
	for (int i = 0; i < poligono.vertices.size(); i++) {
		poligono.vertices[i].first += distancia * cos(angulo);
		poligono.vertices[i].second += distancia * sin(angulo);
	}
	
}

void escalar(Poligono& poligono, double escala_x, double escala_y) {
    // Calcula as coordenadas do ponto fixo em relação à origem
    double xPontoFixo = poligono.posicao.first;
    double yPontoFixo = poligono.posicao.second;

    // Atualiza a escala do polígono
    poligono.escala.first *= escala_x;
    poligono.escala.second *= escala_y;

    // Para cada vértice do polígono, ajusta as coordenadas em relação ao ponto fixo
    for (int i = 0; i < poligono.vertices.size(); i++) {
        // Calcula as coordenadas do vértice em relação ao ponto fixo
        double xVertice = poligono.vertices[i].first - xPontoFixo;
        double yVertice = poligono.vertices[i].second - yPontoFixo;

        // Aplica a escala às coordenadas do vértice
        double xEscalado = xVertice * escala_x;
        double yEscalado = yVertice * escala_y;

        // Atualiza as coordenadas do vértice com base no ponto fixo
        poligono.vertices[i].first = xEscalado + xPontoFixo;
        poligono.vertices[i].second = yEscalado + yPontoFixo;
    }
}

 void rotacionar(Poligono& poligono, double angulo) {
        // Calcula as coordenadas do ponto fixo em relação à origem
        double xPontoFixo = poligono.posicao.first;
        double yPontoFixo = poligono.posicao.second;

        for (int i = 0; i < poligono.vertices.size(); i++) {
            // Calcula as coordenadas do vértice em relação ao ponto fixo
            double xVertice = poligono.vertices[i].first - xPontoFixo;
            double yVertice = poligono.vertices[i].second - yPontoFixo;

            // Aplica a rotação às coordenadas do vértice
            double xRotacionado = xVertice * cos(angulo) - yVertice * sin(angulo);
            double yRotacionado = xVertice * sin(angulo) + yVertice * cos(angulo);

            // Atualiza as coordenadas do vértice com base no ponto fixo
            poligono.vertices[i].first = xRotacionado + xPontoFixo;
            poligono.vertices[i].second = yRotacionado + yPontoFixo;
        }
}

void desenhar(Poligono poligono) {
	// Desenha o poligono
	glColor3f(0.0, 0.0, 0.0);
	// Desenha as arestas do poligono
	glBegin(GL_LINES);
	for (int i = 0; i < poligono.arestas.size(); i++) {
		// Desenha a aresta i do poligono
		float v_o = poligono.arestas[i].first;
		float v_d = poligono.arestas[i].second;
		// Desenha a aresta que liga os vertices v_o e v_d
		glVertex2f(poligono.vertices[v_o].first, poligono.vertices[v_o].second);
		glVertex2f(poligono.vertices[v_d].first, poligono.vertices[v_d].second);
	}
	glEnd();
}