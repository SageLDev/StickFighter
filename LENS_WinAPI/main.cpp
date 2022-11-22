#define WIN32_LEAN_AND_MEAN //No agrega librerías que no se vayan a utilizar

#include <Windows.h>
#include <stdio.h>
#include <dinput.h>
#include <gdiplus.h>
#include <gl/gl.h>
#include <gl/glu.h>

using namespace Gdiplus;

//Variables constantes
/* Variable constante para calcular el ancho de la ventana */
const int ANCHO_VENTANA = 1280;

/* Variable constante para calcular el alto de la ventana */
const int ALTO_VENTANA = 720;

/* Variable constante que define la cantidad de bytes por pixel, usada en las operaciones de desplegar sprites/imagenes en pantalla */
const int BPP = 4;

/* Variable constante que define el intervalo del contador o timer en milisegundos, 
	con cada TICK del contador se ejecuta el codigo dentro del case WM_TIMER en la funcion WndProc */
const int TICK = 100;

/* Variables constantes de los colores primarios de un pixel de 32 bits */
const unsigned int BLUE = 0xFF0000FF;
const unsigned int GREEN = 0xFF00FF00;
const unsigned int RED = 0xFFFF0000;

/* Estructura con las coordenadas de los sprites en pantalla en un plano 2D */
struct POSITION {
	int X;
	int Y;
};

/* Estructura con las dimensiones de los sprites a cargar y desplegar en pantalla */
struct DIMENSION {
	int ANCHO;
	int ALTO;
};

/* Estructura con la enumeracion de algunas teclas.
	Se tiene un objeto o variable del tipo de esta estructura, llamado 'input' 
	que sera para acceder a cada uno de las elementos de la enumeracion; ejemplo:
	input.A para la tecla 'A'.*/
struct Input
{
	enum Keys
	{
		Backspace = 0x08, Tab,
		Clear = 0x0C, Enter,
		Shift = 0x10, Control, Alt,
		Escape = 0x1B,
		Space = 0x20, PageUp, PageDown, End, Home, Left, Up, Right, Down,
		Zero = 0x30, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
		A = 0x41, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		NumPad0 = 0x60, NumPad1, NumPad2, NumPad3, NumPad4, NumPad5, NumPad6, NumPad7, NumPad8, NumPad9, 
		F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
	};
}input /* declaracion del objeto de la estructura Input */;

//Variables Globales
int *ptrBuffer;
unsigned char * ptrBack;
unsigned char * ptrPersonaje1;
unsigned char * ptrPersonaje2;
unsigned char * ptrHud;
unsigned char* ptrVida1;
unsigned char* ptrVida2;
unsigned char* ptrMenu;
unsigned char* ptrP2Win;
unsigned char* ptrP1Win;
unsigned char* ptrM1;
unsigned char* ptrM2;
unsigned char* ptrC1;
unsigned char* ptrC2;
unsigned char* ptrD1;
unsigned char* ptrD2;
unsigned char* ptrU1;
unsigned char* ptrU2;
DIMENSION dmnBack, dmnPersonaje1, dmnPersonaje2, dmnHud, dmnVida1, dmnVida2, dmnMenu, dmnP2Win, dmnP1Win, dmnM1, dmnM2, dmnC1, dmnC2, dmnD1, dmnD2, dmnU1, dmnU2;
POSITION posPer1;
POSITION posPer2;
POSITION posHud;
POSITION posVida1;
POSITION posVida2;
POSITION posMenu;
POSITION posP2Win;
POSITION posP1Win;
POSITION posM1;
POSITION posM2;
POSITION posC1;
POSITION posC2;
POSITION posD1;
POSITION posD2;
POSITION posU1;
POSITION posU2;
int indiPersonaje1 = 1;
int indiPersonaje2 = 1;
int initfondo;
bool KEYS[256];
int increfondo = 0;    //que incremente el valor del pixel 
int contadorsh = 10;

int coloresdif = 0;
bool mirror1;
bool mirror2;
int start;
int scale = 7;

bool sePuedeMover1 = true;
bool sePuedeMover2 = true;

int framesAtaque1 = 0;
int framesAtaque2 = 0;

int framesAux1 = 0;
int framesAux2 = 0;

bool estaAtacando1 = false;
bool estaAtacando2 = false;

int ataqueAdelanto1 = 0;
int ataqueAdelanto2 = 0;

int rango1 = 0;
int rango2 = 0;

int recovery1 = 0;
int recovery2 = 0;

int framesStagger1 = 0;
int framesStagger2 = 0;

bool estaStagger1 = false;
bool estaStagger2 = false;

bool estaBloqueando1 = false;
bool estaBloqueando2 = false;

bool estaEnRecovery1 = false;
bool estaEnRecovery2 = false;

int dano1 = 0;
int dano2 = 0;

int puntosAtaque1 = 0;
int puntosAtaque2 = 0;

int puntuacionTotal1 = 0;
int puntuacionTotal2 = 0;

int vida1 = 500;
int vida2 = 500;

bool fueGolpeado1 = false;
bool fueGolpeado2 = false;

bool invulnerable1 = false;
bool invulnerable2 = false;

bool gameOver = false;
bool gameStart = false;

bool player1win = false;
bool player2win = false;

int m1, c1, d1, u1,m2, c2, d2, u2;


//Declaracion de funciones
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void MainRender(HWND hWnd);
void Init();
void Score();
void KeysEvents();
unsigned char * CargaImagen(WCHAR rutaImagen[], DIMENSION * dmn);
POSITION setPosition(int x, int y);
void DibujaFondo(int *buffer, int *imagen, DIMENSION dmn, int incremento);
void DibujaPersonaje(int *buffer, int *personaje, DIMENSION dmn1, POSITION pos1);
void DibujaPersonaje2(int* buffer, int* personaje, DIMENSION dmn1, POSITION pos1);
void DibujaInterfaz(int *buffer, int *hud, DIMENSION dmn4, POSITION pos4);
void DibujaBarraDeVida1(int* buffer, int* hud, DIMENSION dmn4, POSITION pos4);
void DibujaBarraDeVida2(int* buffer, int* hud, DIMENSION dmn4, POSITION pos4);
void DibujarPantallaInicial(int* buffer, int* hud, DIMENSION dmn4, POSITION pos4);
void P2Win(int* buffer, int* hud, DIMENSION dmn4, POSITION pos4);
void P1Win(int* buffer, int* hud, DIMENSION dmn4, POSITION pos4);
void DibujarMiles1(int* buffer, int* hud, DIMENSION dmn4, POSITION pos4);
void DibujarCentenas1(int* buffer, int* hud, DIMENSION dmn4, POSITION pos4);
void DibujarDecenas1(int* buffer, int* hud, DIMENSION dmn4, POSITION pos4);
void DibujarUnidades1(int* buffer, int* hud, DIMENSION dmn4, POSITION pos4);
void DibujarMiles2(int* buffer, int* hud, DIMENSION dmn4, POSITION pos4);
void DibujarCentenas2(int* buffer, int* hud, DIMENSION dmn4, POSITION pos4);
void DibujarDecenas2(int* buffer, int* hud, DIMENSION dmn4, POSITION pos4);
void DibujarUnidades2(int* buffer, int* hud, DIMENSION dmn4, POSITION pos4);

int WINAPI wWinMain(HINSTANCE hInstance, 
					 HINSTANCE hPrevInstance, 
					 PWSTR pCmdLine, 
					 int nCmdShow)
{
	WNDCLASSEX wc;									// Windows Class Structure
	HWND hWnd;
	MSG msg;

	TCHAR szAppName[] = TEXT("MyWinAPIApp");		
	TCHAR szAppTitle[] = TEXT("LENS App");

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance	
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= (HBRUSH) (COLOR_WINDOW + 1);			// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu	
	wc.lpszClassName	= szAppName;							// Set The Class Name
	wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);
	
	if (!RegisterClassEx(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,
			L"Fallo al registrar clase (Failed To Register The Window Class).",
			L"ERROR",
			MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}

	hWnd = CreateWindowEx(	
		WS_EX_CLIENTEDGE | WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,	// Extended Style For The Window
		szAppName,							// Class Name
		szAppTitle,							// Window Title
		WS_OVERLAPPEDWINDOW |				// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		ANCHO_VENTANA,						// Calculate Window Width
		ALTO_VENTANA,						// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL);								// Pass this class To WM_CREATE								

	if(hWnd == NULL) {
		MessageBox(NULL, 
			L"Error al crear ventana (Window Creation Error).", 
			L"ERROR", 
			MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}
		
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	Init();
	ShowWindow(hWnd, nCmdShow);
	SetFocus(hWnd);

	SetTimer(hWnd, TICK, TICK, NULL);
	ZeroMemory(&msg, sizeof(MSG));

	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return(int)msg.wParam;
}

/* Funcion tipo Callback para el manejo de los eventos de la ventana. 
	*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)									// Check For Windows Messages
	{
		case WM_TIMER:
			if(wParam == TICK)
			{
				MainRender(hWnd);
			}
			break;
		case WM_PAINT:
			{
				HDC hdc; 
				PAINTSTRUCT ps;
				hdc = BeginPaint(hWnd, &ps);

				BITMAP bm;
				HBITMAP h_CMC = CreateBitmap(ANCHO_VENTANA, ALTO_VENTANA, 1, 32, ptrBuffer);
				HDC hdcMem = CreateCompatibleDC(hdc);
				HBITMAP hbmOld = (HBITMAP) SelectObject(hdcMem, h_CMC);
				GetObject(h_CMC, sizeof(bm), &bm);

				BitBlt(hdc, 0, 0, ANCHO_VENTANA, ALTO_VENTANA, hdcMem, 0, 0, SRCCOPY);

				DeleteObject(h_CMC);
				SelectObject(hdcMem, hbmOld);
				DeleteDC(hdcMem);
				DeleteObject(hbmOld);
			}
			break;		
		case WM_KEYDOWN:							
			{
				KEYS[ wParam ] = true;
			}
			break;
		case WM_KEYUP:
			{
				KEYS[ wParam ] = false;
			}
			break;
		case WM_CLOSE: 
			{
				DestroyWindow(hWnd);
			}
			break;
		case WM_DESTROY: //Send A Quit Message
			{
				KillTimer(hWnd, TICK);
				PostQuitMessage(0);
			}
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

/* Funcion usada para la inicializacion de variables y reserva de espacio en memoria.
	*/
void Init() 
{
	posPer1.X = 426;
	posPer1.Y = 115;

	posPer2.X = 900;
	posPer2.Y = 115;
	/*initfondo = 0;*/

	posHud.X = 25;
	posHud.Y = 50;

	posVida1.X = 50;
	posVida1.Y = 79;

	posVida2.X = 700;
	posVida2.Y = 79;

	posMenu.X = 1;
	posMenu.Y =-200;

	posP2Win.X = 1;
	posP2Win.Y = 150;
	posP1Win.X = 1;
	posP1Win.Y = 150;

	posM1.X = 140;
	posM1.Y = 10;

	posC1.X = 100;
	posC1.Y = 10;

	posD1.X = 60;
	posD1.Y = 10;

	posU1.X = 20;
	posU1.Y = 10;

	posM2.X = 1020;
	posM2.Y = 10;

	posC2.X = 1100;
	posC2.Y = 10;	

	posD2.X = 1060;
	posD2.Y = 10;

	posU2.X = 1140;
	posU2.Y = 10;


	for(int i = 0; i < 256; i++)
	{
		KEYS[i] = false;
	}

	//Inicializar el puntero tipo int 'ptrBuffer' que contiene la direccion inicial  del area de memoria reservada para el despliegue de sprites/imagenes.
	ptrBuffer = new int[ANCHO_VENTANA * ALTO_VENTANA];

	//Inicializar el puntero tipo unsigned char 'ptrBack' que contiene la direccion inicial en memoria del arreglo de pixeles de la imagen especificada en el primer parametro
	//y en la variable dmnBack de tipo DIMENSION* estan los valores de ANCHO y ALTO de la imagen.
	ptrBack = CargaImagen(TEXT("./Stages/stage_roof.png"), &dmnBack); //puntero a la imagen
	ptrPersonaje1 = CargaImagen(TEXT("./Animations/animation_forward.png"), &dmnPersonaje1); 
	ptrPersonaje2 = CargaImagen(TEXT("./Animations/animation_forward.png"), &dmnPersonaje2); //puntero a mi personaje sprite
	ptrHud = CargaImagen(TEXT("./HUD/hud_life.png"), &dmnHud);
	ptrVida1 = CargaImagen(TEXT("./HUD/hud_life_bar.png"), &dmnVida1);
	ptrVida2 = CargaImagen(TEXT("./HUD/hud_life_bar.png"), &dmnVida2);
	ptrMenu = CargaImagen(TEXT("./HUD/menu.png"), &dmnMenu);
	ptrP2Win = CargaImagen(TEXT("./HUD/victoria2.png"), &dmnP2Win);
	ptrP1Win = CargaImagen(TEXT("./HUD/victoria1.png"), &dmnP1Win);
	ptrM1 = CargaImagen(TEXT("./HUD/0.png"), &dmnM1);
	ptrM2 = CargaImagen(TEXT("./HUD/0.png"), &dmnM2);
	ptrC1 = CargaImagen(TEXT("./HUD/0.png"), &dmnC1);
	ptrC2 = CargaImagen(TEXT("./HUD/0.png"), &dmnC2);
	ptrD1 = CargaImagen(TEXT("./HUD/0.png"), &dmnD1);
	ptrD2 = CargaImagen(TEXT("./HUD/0.png"), &dmnD2);
	ptrU1 = CargaImagen(TEXT("./HUD/0.png"), &dmnU1);
	ptrU2 = CargaImagen(TEXT("./HUD/0.png"), &dmnU2);
}

/* Funcion principal. Encargada de hacer el redibujado en pantalla cada intervalo (o "tick") del timer que se haya creado.
	@param hWnd. Manejador de la ventana.
	*/
void MainRender(HWND hWnd) 
{
	KeysEvents();
	
	DibujaFondo(ptrBuffer, (int*)ptrBack, dmnBack, increfondo);
	Score();
	if (player2win == true) {
		P2Win(ptrBuffer, (int*)ptrP2Win, dmnP2Win, posP2Win);
	} 

	if (player1win == true) {
		P1Win(ptrBuffer, (int*)ptrP1Win, dmnP1Win, posP1Win);
	}

	if (gameStart && !gameOver) {

		DibujaInterfaz(ptrBuffer, (int*)ptrHud, dmnHud, posHud);

		DibujaBarraDeVida1(ptrBuffer, (int*)ptrVida1, dmnVida1, posVida1);

		DibujaBarraDeVida2(ptrBuffer, (int*)ptrVida2, dmnVida2, posVida2);
	}

	if (gameStart || gameOver) {

		DibujarMiles1(ptrBuffer, (int*)ptrM1, dmnM1, posM1);
		DibujarCentenas1(ptrBuffer, (int*)ptrC1, dmnC1, posC1);
		DibujarDecenas1(ptrBuffer, (int*)ptrD1, dmnD1, posD1);
		DibujarUnidades1(ptrBuffer, (int*)ptrU1, dmnU1, posU1);

		DibujarMiles2(ptrBuffer, (int*)ptrM2, dmnM2, posM2);
		DibujarCentenas2(ptrBuffer, (int*)ptrC2, dmnC2, posC2);
		DibujarDecenas2(ptrBuffer, (int*)ptrD2, dmnD2, posD2);
		DibujarUnidades2(ptrBuffer, (int*)ptrU2, dmnU2, posU2);

		DibujaPersonaje(ptrBuffer, (int*)ptrPersonaje1, dmnPersonaje1, posPer1);

		DibujaPersonaje2(ptrBuffer, (int*)ptrPersonaje2, dmnPersonaje2, posPer2);
	}

	if (!gameStart || gameOver) {
		DibujarPantallaInicial(ptrBuffer, (int*)ptrMenu, dmnMenu, posMenu);
	}
	

	

	//Funciones que deberan estar el final de la funcion de Render.
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
}

/* Funcion que regresa la posicion del sprite en pantalla.
	@param x. Coordenada X en la ventana.
	@param y. Coordenada Y en la ventana.
	*/
POSITION setPosition(int x, int y) {
	POSITION p;
	p.X = x;
	p.Y = y;
	return p;
}

/* Funcion para manejar eventos del teclado dependiendo de la(s) tecla(s) que se haya(n) presionado.
	*/
void KeysEvents() 
{

	if (KEYS[input.Backspace]) {
		if (gameOver || !gameStart) {

			posPer1.X = 426;
			posPer1.Y = 115;

			posPer2.X = 900;
			posPer2.Y = 115;

			vida1 = 500;
			vida2 = 500;

			puntuacionTotal1 = 0;
			puntuacionTotal2 = 0;

			sePuedeMover1 = true;
			sePuedeMover2 = true;

			player1win = false;
			player2win = false;
			
			gameOver = false;
			gameStart = true;
		}
	}

	if ((KEYS[input.A] || KEYS[input.D]) && KEYS[input.Space]) {

		if (sePuedeMover1 == true) {
			indiPersonaje1 = 0;
			sePuedeMover1 = false;
			estaAtacando1 = true;
			ptrPersonaje1 = CargaImagen(TEXT("./Animations/animation_ataque_1.png"), &dmnPersonaje1);
			mirror1 = false;
			framesAtaque1 = 5;
			ataqueAdelanto1 = 10;
			rango1 = 140;
			recovery1 = 2;
			dano1 = 51;
			puntosAtaque1 = 100;
		}
	}
	else if (KEYS[input.Space]) {
		if (sePuedeMover1 == true) {
			indiPersonaje1 = 0;
			sePuedeMover1 = false;
			estaAtacando1 = true;
			ptrPersonaje1 = CargaImagen(TEXT("./Animations/animation_ataque_0.png"), &dmnPersonaje1);
			mirror1 = false;
			framesAtaque1 = 5;
			ataqueAdelanto1 = 20;
			rango1 = 180;
			recovery1 = 5;
			dano1 = 101;
			puntosAtaque1 = 300;
		}
	}

	if (estaAtacando1) {
		indiPersonaje1++;
		indiPersonaje1 = indiPersonaje1 >= framesAtaque1 ? 0 : indiPersonaje1;
		framesAux1++;
		posPer1.X += ataqueAdelanto1;

		if ((posPer1.X + rango1) > posPer2.X) {
			indiPersonaje2 = 0;
  			if (estaBloqueando2 == true) {
				if (!invulnerable2) {
					sePuedeMover2 = false;
					estaStagger2 = true;
					posPer2.X += 30;
					posPer1.X -= 20;
				}
				
			}
			else {
				if (!invulnerable2) {
					sePuedeMover2 = false;
					fueGolpeado2 = true;
					posPer2.X += 30;
					posPer1.X -= 30;
				}	
			}
		}

		if (framesAux1 == framesAtaque1) {
			estaAtacando1 = false;
			estaEnRecovery1 = true;
			framesAux1 = 0;
		}


	}

	if (estaEnRecovery1) {

		framesAux1++;
		if (framesAux1 == recovery1) {
			sePuedeMover1 = true;
			estaEnRecovery1 = false;
			framesAux1 = 0;
			ataqueAdelanto1 = 0;
			rango1 = 0;
			recovery1 = 0;
			dano1 = 0;
			puntosAtaque1 = 0;
		}

	
	}

	if (estaStagger1) {

		ptrPersonaje1 = CargaImagen(TEXT("./Animations/animation_block.png"), &dmnPersonaje1);
		mirror1 = false;
		indiPersonaje1++;
		indiPersonaje1 = indiPersonaje1 >= 2 ? 0 : indiPersonaje1;
		invulnerable1 = true;
		framesAux1++;

		if (framesAux1 == 7) {

			sePuedeMover1 = true;
			estaStagger1 = false;
			invulnerable1 = false;
			framesAux1 = 0;
		}
	}

	if (fueGolpeado1) {

		if (vida1 > 0) {
			ptrPersonaje1 = CargaImagen(TEXT("./Animations/animation_damage.png"), &dmnPersonaje1);
			mirror1 = false;
			indiPersonaje1++;
			indiPersonaje1 = indiPersonaje1 >= 4 ? 0 : indiPersonaje1;
			framesAux1++;
			invulnerable1 = true;

			if (framesAux1 == 7) {
				vida1 -= dano2;
				puntuacionTotal2 += puntosAtaque2;
				sePuedeMover1 = true;
				fueGolpeado1 = false;
				invulnerable1 = false;
				framesAux1 = 0;
			}
		}
		else if (vida1 <= 0) {
			sePuedeMover1 = false;
			sePuedeMover2 = false;
			ptrPersonaje1 = CargaImagen(TEXT("./Animations/animation_dead.png"), &dmnPersonaje1);
			mirror1 = false;
			indiPersonaje1++;
			framesAux1++;

			if (indiPersonaje1 == 7) {
				fueGolpeado1 = false;
				gameOver = true;
				framesAux1 = 0;
				player2win = true;
			}

		}


	}

	if (sePuedeMover1) {
		
		if (KEYS[input.D] == false && KEYS[input.A] == false) {

			ptrPersonaje1 = CargaImagen(TEXT("./Animations/animation_idle.png"), &dmnPersonaje1);
			mirror1 = FALSE;
			indiPersonaje1++;
			indiPersonaje1 = indiPersonaje1 >= 5 ? 0 : indiPersonaje1;

		}

		if (KEYS[input.D]) {
			ptrPersonaje1 = CargaImagen(TEXT("./Animations/animation_forward.png"), &dmnPersonaje1);
			mirror1 = FALSE;
			indiPersonaje1++;
			indiPersonaje1 = indiPersonaje1 >= 6 ? 0 : indiPersonaje1;
			if (posPer1.X < 1200)   //para que tope en mi ventana de 800 del lado derecho
			{
				posPer1.X += 30;
			}
			else
			{
				if (increfondo < 100)  //para que tope en mi imagen 2048
				{
					increfondo += 10;  //velocidad en que avanza
				}
			}
		}

		if (KEYS[input.A]) {
			ptrPersonaje1 = CargaImagen(TEXT("./Animations/animation_backwards.png"), &dmnPersonaje1);
			mirror1 = FALSE;
			indiPersonaje1++;
			indiPersonaje1 = indiPersonaje1 >= 6 ? 0 : indiPersonaje1;
			estaBloqueando1 = true;
			if (posPer1.X > 150)  //para que tope en mi ventana de 800 del lado izquierdo
			{
				posPer1.X -= 30;
			}
			else
			{
				if (increfondo > 10)  //para que tope en mi imagen 2048
				{
					increfondo -= 10;  //velocidad que se regresa
				}
			}
		}

		if (GetAsyncKeyState('A') == 0 && estaBloqueando1 == true)
		{
			estaBloqueando1 = false;
		}
	}

	

	//Player 2
	
	if ((KEYS[input.Right] || KEYS[input.Left]) && KEYS[input.Enter]) {

		if (sePuedeMover2 == true) {
			indiPersonaje2 = 0;
			sePuedeMover2 = false;
			estaAtacando2 = true;
			ptrPersonaje2 = CargaImagen(TEXT("./Animations/animation_ataque_1.png"), &dmnPersonaje2);
			mirror2 = true;
			framesAtaque2 = 5;
			ataqueAdelanto2 = 10;
			rango2 = 140;
			recovery2 = 2;
			dano2 = 51;
			puntosAtaque2 = 100;
		}
	}
	else if (KEYS[input.Enter]) {
		if (sePuedeMover2 == true) {
			indiPersonaje2 = 0;
			sePuedeMover2 = false;
			estaAtacando2 = true;
			ptrPersonaje2 = CargaImagen(TEXT("./Animations/animation_ataque_0.png"), &dmnPersonaje2);
			mirror2 = true;
			framesAtaque2 = 5;
			ataqueAdelanto2 = 20;
			rango2 = 180;
			recovery2 = 5;
			dano2 = 101;
			puntosAtaque2 = 300;
		}
	}

	if (estaAtacando2) {
		indiPersonaje2++;
		indiPersonaje2 = indiPersonaje2 >= framesAtaque2 ? 0 : indiPersonaje2;
		framesAux2++;
		posPer2.X -= ataqueAdelanto2;

		if ((posPer2.X - rango2) < posPer1.X) {
			indiPersonaje1 = 0;
			if (estaBloqueando1 == true) {
				if (!invulnerable1) {
					sePuedeMover1 = false;
					estaStagger1 = true;
					posPer1.X += 30;
					posPer2.X -= 20;
				}

			}
			else {
				if (!invulnerable1) {
					sePuedeMover1 = false;
					fueGolpeado1 = true;
					posPer1.X += 30;
					posPer2.X -= 30;
				}
			}
		}

		if (framesAux2 == framesAtaque2) {
			estaAtacando2 = false;
			estaEnRecovery2 = true;
			framesAux2 = 0;
		}


	}

	if (estaEnRecovery2) {

		framesAux2++;
		if (framesAux2 == recovery2) {
			sePuedeMover2 = true;
			estaEnRecovery2 = false;
			framesAux2 = 0;
			ataqueAdelanto2 = 0;
			rango2 = 0;
			recovery2 = 0;
			dano2 = 0;
			puntosAtaque2 = 0;
		}


	}

	if (estaStagger2) {
		
		ptrPersonaje2 = CargaImagen(TEXT("./Animations/animation_block.png"), &dmnPersonaje2);
		mirror2 = TRUE;
		indiPersonaje2++;
		indiPersonaje2 = indiPersonaje2 >= 2 ? 0 : indiPersonaje2;
		invulnerable2 = true;
		framesAux2++;

		if (framesAux2 == 7) {

			sePuedeMover2 = true;
			estaStagger2 = false;
			invulnerable2 = false;
			framesAux2 = 0;
		}
	}

	if (fueGolpeado2) {
		
		if (vida2 > 0) {			
			ptrPersonaje2 = CargaImagen(TEXT("./Animations/animation_damage.png"), &dmnPersonaje2);
			mirror2 = TRUE;
			indiPersonaje2++;
			indiPersonaje2 = indiPersonaje2 >= 4 ? 0 : indiPersonaje2;
			framesAux2++;
			invulnerable2 = true;

			if (framesAux2 == 7) {
				vida2 -= dano1;
				puntuacionTotal1 += puntosAtaque1;
				sePuedeMover2 = true;
				fueGolpeado2 = false;
				invulnerable2 = false;
				framesAux2 = 0;
			}
		}
		else if (vida2 <= 0) {
			sePuedeMover1 = false;
			sePuedeMover2 = false;
			ptrPersonaje2 = CargaImagen(TEXT("./Animations/animation_dead.png"), &dmnPersonaje2);
			mirror2 = TRUE;
			indiPersonaje2++;
			framesAux2++;

			if (indiPersonaje2 == 8) {
				fueGolpeado2 = false;
				gameOver = true;
				framesAux2 = 0;
				player1win = true;
			}

		}

		
	}

	if (sePuedeMover2 == true) {



		if (KEYS[input.Right] == false && KEYS[input.Left] == false) {

			ptrPersonaje2 = CargaImagen(TEXT("./Animations/animation_idle.png"), &dmnPersonaje2);
			mirror2 = TRUE;
			indiPersonaje2++;
			indiPersonaje2 = indiPersonaje2 >= 5 ? 0 : indiPersonaje2;

		} 

		if (KEYS[input.Right]) {
			estaBloqueando2 = true;
			ptrPersonaje2 = CargaImagen(TEXT("./Animations/animation_backwards.png"), &dmnPersonaje2);
			mirror2 = TRUE;
			indiPersonaje2++;
			indiPersonaje2 = indiPersonaje2 >= 6 ? 0 : indiPersonaje2;
			if (posPer2.X < 1200)   //para que tope en mi ventana de 800 del lado derecho
			{
				posPer2.X += 30;
			}
			else
			{
				if (increfondo < 100)  //para que tope en mi imagen 2048
				{
					increfondo += 10;  //velocidad en que avanza
				}
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) == 0 && estaBloqueando2 == true)
		{
			estaBloqueando2 = false;
		}

		if (KEYS[input.Left]) {

			
			ptrPersonaje2 = CargaImagen(TEXT("./Animations/animation_forward.png"), &dmnPersonaje2);
			mirror2 = TRUE;
			indiPersonaje2++;
			indiPersonaje2 = indiPersonaje2 >= 6 ? 0 : indiPersonaje2;
			if (posPer2.X > 150)  //para que tope en mi ventana de 800 del lado izquierdo
			{
				posPer2.X -= 30;
			}
			else
			{
				if (increfondo > 10)  //para que tope en mi imagen 2048
				{
					increfondo -= 10;  //velocidad que se regresa
				}
			}
		}


	}





}

/* Funcion para cargar imagenes y obtener un puntero al area de memoria reservada para la misma.
	@param rutaImagen.			Nombre o ruta de la imagen a cargar en memoria.
	@return unsigned char *.	Direccion base de la imagen.
	*/
unsigned char * CargaImagen(WCHAR rutaImagen[], DIMENSION * dmn)
{
	unsigned char * ptrImagen;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR  gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Bitmap *bitmap=new Bitmap(rutaImagen);
	BitmapData *bitmapData=new BitmapData;

	dmn->ANCHO = bitmap->GetWidth();
	dmn->ALTO = bitmap->GetHeight();

	Rect rect(0, 0, dmn->ANCHO, dmn->ALTO);

	//Reservamos espacio en memoria para la imagen
	bitmap->LockBits(&rect, ImageLockModeRead, PixelFormat32bppRGB, bitmapData);

	//"pixels" es el puntero al area de memoria que ocupa la imagen
	unsigned char* pixels = (unsigned char*)bitmapData->Scan0;

	//"tamaño" lo usaremos para reservar los bytes que necesita la imagen. 
	//Para calcular la cantidad de bytes total necesitamos multiplicamos el area de la imagen * 4. 
	//Se multiplica por 4 debido a que cada pixel ocupa 4 bytes de memoria. Noten el 3er parametro de la funcion LockBits, dos lineas de codigo arriba.
	//PixelFormat32bppARGB -> Specifies that the format is 32 bits per pixel; 8 bits each are used for the alpha, red, green, and blue components.
	//Mas info: https://msdn.microsoft.com/en-us/library/system.drawing.imaging.pixelformat(v=vs.110).aspx
	int tamaño;
	tamaño = dmn->ANCHO * dmn->ALTO * 4;
	//hagamos un try de la reserva de memoria
	try
	{
		ptrImagen = new unsigned char [tamaño]; 
	}
	catch(...)
	{
		return NULL;
	}

	//Después de este for, ptrImagen contiene la direccion en memoria de la imagen.
	for(int i=0, j=tamaño; i < j; i++)
	{
		ptrImagen[i]=pixels[i];
	}

	//Es necesario liberar el espacio en memoria, de lo contrario marcaria una excepcion de no hay espacio de memoria suficiente.
	bitmap->UnlockBits(bitmapData);
	delete bitmapData;
	delete bitmap;
	  
	GdiplusShutdown(gdiplusToken);

	return ptrImagen;
}

#pragma region LENS_CODE
void DibujaFondo(int * buffer, int * imagen, DIMENSION dmn, int incremento) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO;
	__asm {

		cld  
		mov esi, imagen   
		mov edi, buffer   

		mov ecx, ALTO_VENTANA 

		mov eax, incremento   //mi parametro para incrementar los pixeles, lo mando a eax
		mul BPP               //multiplico por los bytes
		add esi, eax          //se lo agrego a mi imagen para que se recorra

	PonerA :
		push ecx  //el alto lo pongo al inicio de mi linea
			mov ecx, ANCHO_VENTANA //recorre el ancho

		PonerL :
		mov eax, [esi]     //mi imagen la paso a eax
			mov[edi], eax      //eax lo pongo en mi ventana  (paso mi imagen a la ventana)
			add edi, BPP                     //incremento mi pixel 4 bytes
			add esi, BPP
			loop PonerL
			mov eax, 1424                    //ancho de mi imagen
			mul BPP                          //multiplico por 4 bytes
			add esi, eax                     //incremento para que me pase a la linea de abajo
			mov eax, ANCHO_VENTANA
			mul BPP
			sub esi, eax                     //resto para posicionarlo al principio de mi linea
			pop ecx   //para sacar el alto y restarle uno
			loop PonerA

		//; Inicializar registros indices
		//	; con la direccion inicial en memoria de la imagen a desplegar(fuente->imagen->registro indice fuente->ESI)
		//	; y del area de memoria reservada para el despliegue(destino->buffer->registro indice destino->EDI)
		//	; mov esi, imagen
		//	; mov edi, buffer
		//	; Cargar la direccion en memoria de la variable 'dmn' el registro base(EBX). 'dmn' es la estructura con el ANCHO y ALTO.
		//	; lea ebx, dmn
		//;Transferir o copiar 4 bytes en memoria a partir de la direccion en memoria especificada por EBX; 
		//; son 4 bytes porque el operando destino es de 4 bytes en este caso es el registro acumulador(EAX).
		//	; Es decir, EBX es un puntero a 'dmn' donde 'dmn' es de 8 bytes(4Bytes de la variable ANCHO y los siguientes 4Bytes de la variable ALTO),
		//	; entonces EAX = ANCHO
		//	; mov eax, [ebx]
		//	; sumar 4 al registro puntero(EBX) para apuntar al siguiente elemento de la estructura DIMENSION el cual es ALTO y vendria siendo el otro factor de la multiplicacion
		//	; add ebx, 4
		//	; Especificar el otro factor en la instruccion de multiplicacion que son los siguientes 4Bytes a partir de la direccion especificada por EBX(variable ALTO de DIMENSION)
		//	; Se multplican ANCHO(= EAX) * ALTO(= dword ptr[ebx])
		//	; con 'dword ptr [ebx]' estamos indicando que EBX se comporte como puntero tipo DWORD,
		//	; es decir, se esta especificando que se desean los 4Bytes (4Bytes = 1Dword) a partir de la direccion en memoria especificada por el registro EBX
		//	; el cual contiene la direccion inicial de la variable ALTO en la estructura DIMENSION.
		//	; mul dword ptr[ebx]
		//	; el resultado de la multiplicacion anterior es del doble del tamano de los factores, en este caso los factores son de 4Bytes por lo que el resultado es de 8Bytes
		//	; y se encuentra en EDX : EAX donde EDX contiene los 4Bytes de mayor peso del producto de la multiplicacion y EAX contiene los 4Bytes de menor peso.
		//	; Como el producto de la multiplicacion sabemos y esperamos que es un valor con un tamano menor a 4Bytes entonces dicho resultado esta en EAX
		//	; y con la siguiente instruccion se lo copiamos al registro contador(ECX)
		//	; para posteriormente hacer el ciclo / bucle de leer cada pixel de la imagen en memoria y pasarlo al buffer(area de mem.reservada para el despliegue)
		//	; mov ecx, eax
		//;poner codigo para el bucle de leer pixel por pixel y transferirlo al buffer.
	}
}

void DibujaInterfaz(int* buffer, int* hud, DIMENSION dmn, POSITION pos)
{
	int w = dmn.ANCHO;
	int h = dmn.ALTO - 1;
	int posX = pos.X;
	int posY = pos.Y;

	__asm
	{
		cld
		mov esi, hud
		mov edi, buffer

		mov eax, posX
		mul BPP
		add edi, eax
		mov eax, ANCHO_VENTANA
		mul BPP
		mul posY
		add edi, eax

		mov eax, w
		mul BPP
		add esi, eax

		xor ecx, ecx
		mov ecx, h

		repite2 :

		push ecx
			mov ecx, w


			mostrar2 :
		mov eax, [esi]
			cmp eax, 0FFFF0000h
			je Color2
			mov[edi], eax


			Color2 :

		add esi, BPP
			add edi, BPP


			loop mostrar2
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			MOV EAX, w
			MUL BPP
			SUB EDI, EAX

			mov eax, w
			mul BPP

			add esi, eax
			mov eax, w
			mul BPP
			sub esi, eax



			pop ecx

			loop repite2
	}

}

void DibujaBarraDeVida1(int* buffer, int* hud, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	if (w > 0) {
		w = vida1;
	}
	if (w <= 0) {
		w = 1;
	}

	int h = dmn.ALTO - 1;
	int posX = pos.X;
	int posY = pos.Y;

	__asm
	{
		cld
		mov esi, hud
		mov edi, buffer

		mov eax, posX
		mul BPP
		add edi, eax
		mov eax, ANCHO_VENTANA
		mul BPP
		mul posY
		add edi, eax

		mov eax, w
		mul BPP
		add esi, eax

		xor ecx, ecx
		mov ecx, h

		repite2 :

		push ecx
			mov ecx, w


			mostrar2 :
		mov eax, [esi]
			cmp eax, 0FFFF0000h
			je Color2
			mov[edi], eax


			Color2 :

		add esi, BPP
			add edi, BPP


			loop mostrar2
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			MOV EAX, w
			MUL BPP
			SUB EDI, EAX

			mov eax, w
			mul BPP

			add esi, eax
			mov eax, w
			mul BPP
			sub esi, eax



			pop ecx

			loop repite2
	}
}

void DibujaBarraDeVida2(int* buffer, int* hud, DIMENSION dmn, POSITION pos) {

	int w = dmn.ANCHO;
	if (w > 0) {
		w = vida2;
	}
	if(w <= 0) {
		w = 1;
	}

	int h = dmn.ALTO - 1;
	int posX = pos.X;
	int posY = pos.Y;

	__asm
	{
		cld
		mov esi, hud
		mov edi, buffer

		mov eax, posX
		mul BPP
		add edi, eax
		mov eax, ANCHO_VENTANA
		mul BPP
		mul posY
		add edi, eax

		mov eax, w
		mul BPP
		add esi, eax

		xor ecx, ecx
		mov ecx, h

		repite2 :

		push ecx
			mov ecx, w


			mostrar2 :
		mov eax, [esi]
			cmp eax, 0FFFF0000h
			je Color2
			mov[edi], eax


			Color2 :

		add esi, BPP
			add edi, BPP


			loop mostrar2
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			MOV EAX, w
			MUL BPP
			SUB EDI, EAX

			mov eax, w
			mul BPP

			add esi, eax
			mov eax, w
			mul BPP
			sub esi, eax



			pop ecx

			loop repite2
	}
}

void DibujarPantallaInicial(int* buffer, int* hud, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO - 1;
	int posX = pos.X;
	int posY = pos.Y;

	__asm
	{
		cld
		mov esi, hud
		mov edi, buffer

		mov eax, posX
		mul BPP
		add edi, eax
		mov eax, ANCHO_VENTANA
		mul BPP
		mul posY
		add edi, eax

		mov eax, w
		mul BPP
		add esi, eax

		xor ecx, ecx
		mov ecx, h

		repite2 :

		push ecx
			mov ecx, w


			mostrar2 :
		mov eax, [esi]
			cmp eax, 0FFFF0000h
			je Color2
			mov[edi], eax


			Color2 :

		add esi, BPP
			add edi, BPP


			loop mostrar2
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			MOV EAX, w
			MUL BPP
			SUB EDI, EAX

			mov eax, w
			mul BPP

			add esi, eax
			mov eax, w
			mul BPP
			sub esi, eax



			pop ecx

			loop repite2
	}
}

void P1Win(int* buffer, int* hud, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO - 1;
	int posX = pos.X;
	int posY = pos.Y;

	__asm
	{
		cld
		mov esi, hud
		mov edi, buffer

		mov eax, posX
		mul BPP
		add edi, eax
		mov eax, ANCHO_VENTANA
		mul BPP
		mul posY
		add edi, eax

		mov eax, w
		mul BPP
		add esi, eax

		xor ecx, ecx
		mov ecx, h

		repite2 :

		push ecx
			mov ecx, w


			mostrar2 :
		mov eax, [esi]
			cmp eax, 0FFFF0000h
			je Color2
			mov[edi], eax


			Color2 :

		add esi, BPP
			add edi, BPP


			loop mostrar2
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			MOV EAX, w
			MUL BPP
			SUB EDI, EAX

			mov eax, w
			mul BPP

			add esi, eax
			mov eax, w
			mul BPP
			sub esi, eax



			pop ecx

			loop repite2
	}
}



void P2Win(int* buffer, int* hud, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO - 1;
	int posX = pos.X;
	int posY = pos.Y;

	__asm
	{
		cld
		mov esi, hud
		mov edi, buffer

		mov eax, posX
		mul BPP
		add edi, eax
		mov eax, ANCHO_VENTANA
		mul BPP
		mul posY
		add edi, eax

		mov eax, w
		mul BPP
		add esi, eax

		xor ecx, ecx
		mov ecx, h

		repite2 :

		push ecx
			mov ecx, w


			mostrar2 :
		mov eax, [esi]
			cmp eax, 0FFFF0000h
			je Color2
			mov[edi], eax


			Color2 :

		add esi, BPP
			add edi, BPP


			loop mostrar2
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			MOV EAX, w
			MUL BPP
			SUB EDI, EAX

			mov eax, w
			mul BPP

			add esi, eax
			mov eax, w
			mul BPP
			sub esi, eax



			pop ecx

			loop repite2
	}
}

void DibujaPersonaje(int *buffer, int *personaje, DIMENSION dmn, POSITION pos)
{
	int w = dmn.ANCHO;
	int h = dmn.ALTO;
	int x = posPer1.X;
	int y = posPer1.Y;


		if (mirror1 == FALSE)
		{
			__asm{
				cld
					//cargo el sprite
					mov esi, personaje
					mov edi, buffer

					//posicionar mi sprite		
					mov eax, y
					mov ebx, 9200
					mul ebx
					add edi, eax

					mov eax, x
					mul BPP
					add edi, eax

					add edi, 769000


					mov eax, 48   //porque es la cantidad de pixeles de cada cuadrito
					mul BPP
					mul indiPersonaje1    //para moverlo con la tecla que asigne, si le pico una ves el 0 default se convierte en uno, de multiplica por 59 y se mueve el sprite
					add esi, eax

					xor ecx, ecx
					mov ecx, h    //asigno mi altura a ecx
					ScaladoY :
				push ecx
					mov ecx, scale
					repite :
				push ecx
					mov ecx, 48
					mostrar :
					mov eax, [esi]         //mi imagen la paso a eax
					cmp eax, 0FFFF0000h    //comparo lo rojo 
					je colorro   //si es igual salta a colorro
					or eax, coloresdif
					push ecx
					mov ecx, scale
					scalax :
				mov[edi], eax
					add edi, BPP
					loop scalax
					pop ecx
					jmp Salir
			colorro :
					mov eax, BPP
					mul scale
					add edi, eax
			Salir :
					add esi, BPP
				loop mostrar
					mov eax, ANCHO_VENTANA
					mul BPP
					add edi, eax
					MOV EAX, 48
					MUL BPP
					sub esi, eax
					mul scale
					SUB EDI, EAX
					pop ecx
					loop repite
					mov eax, w
					mul BPP
					add esi, eax
			
					pop ecx
					loop ScaladoY
			}

			
		}
		else{
			__asm {
				cld
				//cargo el sprite
				mov esi, personaje
				mov edi, buffer

				//posicionar mi sprite		
				mov eax, y
				mov ebx, 9200
				mul ebx
				add edi, eax

				mov eax, x
				mul BPP
				add edi, eax

				add edi, 769000


				mov eax, 48   //porque es la cantidad de pixeles de cada cuadrito
				mul BPP
				mul indiPersonaje1    //para moverlo con la tecla que asigne, si le pico una ves el 0 default se convierte en uno, de multiplica por 59 y se mueve el sprite
				add esi, eax

				xor ecx, ecx
				mov ecx, h    //asigno mi altura a ecx
				ScaladoY2 :
				push ecx
					mov ecx, scale
					repite2 :
				push ecx
					mov ecx, 48
					mostrar2 :
					mov eax, [esi]
					cmp eax, 0FFFF0000h
					je Color2
					or eax, coloresdif
					push ecx
					mov ecx, scale
					scalax2 :
				mov[edi], eax
					add edi, BPP
					loop scalax2
					pop ecx
					jmp Salir2
					Color2 :
				mov eax, BPP
					mul scale
					add edi, eax
					Salir2 :
				sub esi, BPP//
					loop mostrar2
					mov eax, ANCHO_VENTANA
					mul BPP
					add edi, eax
					MOV EAX, 48
					MUL BPP
					add esi, eax
					mul scale
					SUB EDI, EAX
					pop ecx
					loop repite2
					mov eax, w
					mul BPP
					add esi, eax

					pop ecx
					loop ScaladoY2
			}
		}
	
}

void DibujaPersonaje2(int* buffer, int* personaje, DIMENSION dmn, POSITION pos)
{
	int w = dmn.ANCHO;
	int h = dmn.ALTO;
	int x = posPer2.X;
	int y = posPer2.Y;


	if (mirror2 == FALSE)
	{
		__asm {
			cld
			//cargo el sprite
			mov esi, personaje
			mov edi, buffer

			//posicionar mi sprite		
			mov eax, y
			mov ebx, 9200
			mul ebx
			add edi, eax

			mov eax, x
			mul BPP
			add edi, eax

			add edi, 769000


			mov eax, 48   //porque es la cantidad de pixeles de cada cuadrito
			mul BPP
			mul indiPersonaje2    //para moverlo con la tecla que asigne, si le pico una ves el 0 default se convierte en uno, de multiplica por 59 y se mueve el sprite
			add esi, eax

			xor ecx, ecx
			mov ecx, h    //asigno mi altura a ecx
			ScaladoY :
			push ecx
				mov ecx, scale
				repite :
			push ecx
				mov ecx, 48
				mostrar :
				mov eax, [esi]         //mi imagen la paso a eax
				cmp eax, 0FFFF0000h    //comparo lo rojo 
				je colorro   //si es igual salta a colorro
				or eax, coloresdif
				push ecx
				mov ecx, scale
				scalax :
			mov[edi], eax
				add edi, BPP
				loop scalax
				pop ecx
				jmp Salir
				colorro :
			mov eax, BPP
				mul scale
				add edi, eax
				Salir :
			add esi, BPP
				loop mostrar
				mov eax, ANCHO_VENTANA
				mul BPP
				add edi, eax
				MOV EAX, 48
				MUL BPP
				sub esi, eax
				mul scale
				SUB EDI, EAX
				pop ecx
				loop repite
				mov eax, w
				mul BPP
				add esi, eax

				pop ecx
				loop ScaladoY
		}


	}
	else {
		__asm {
			cld
			//cargo el sprite
			mov esi, personaje
			mov edi, buffer

			//posicionar mi sprite		
			mov eax, y
			mov ebx, 9200
			mul ebx
			add edi, eax

			mov eax, x
			mul BPP
			add edi, eax

			add edi, 769000


			mov eax, 48   //porque es la cantidad de pixeles de cada cuadrito
			mul BPP
			mul indiPersonaje2    //para moverlo con la tecla que asigne, si le pico una ves el 0 default se convierte en uno, de multiplica por 59 y se mueve el sprite
			add esi, eax

			xor ecx, ecx
			mov ecx, h    //asigno mi altura a ecx
			ScaladoY2 :
			push ecx
				mov ecx, scale
				repite2 :
			push ecx
				mov ecx, 48
				mostrar2 :
				mov eax, [esi]
				cmp eax, 0FFFF0000h
				je Color2
				or eax, coloresdif
				push ecx
				mov ecx, scale
				scalax2 :
			mov[edi], eax
				add edi, BPP
				loop scalax2
				pop ecx
				jmp Salir2
				Color2 :
			mov eax, BPP
				mul scale
				add edi, eax
				Salir2 :
			sub esi, BPP//
				loop mostrar2
				mov eax, ANCHO_VENTANA
				mul BPP
				add edi, eax
				MOV EAX, 48
				MUL BPP
				add esi, eax
				mul scale
				SUB EDI, EAX
				pop ecx
				loop repite2
				mov eax, w
				mul BPP
				add esi, eax

				pop ecx
				loop ScaladoY2
		}
	}

}

void Score() {

	u1 = puntuacionTotal1 % 10;
	c1 = (puntuacionTotal1 / 10) % 10;
	d1 = (puntuacionTotal1 / 100) % 10;
	m1 = puntuacionTotal1 / 1000;
	
	u2 = puntuacionTotal2 % 10;
	c2 = (puntuacionTotal2 / 10) % 10;
	d2 = (puntuacionTotal2 / 100) % 10;
	m2 = puntuacionTotal2 / 1000;


	switch (u1)
	{
	case 0:
		ptrU1 = CargaImagen(TEXT("./HUD/0.png"), &dmnU1);
		break;
	case 1:
		ptrU1 = CargaImagen(TEXT("./HUD/1.png"), &dmnU1);
		break;
	case 2:
		ptrU1 = CargaImagen(TEXT("./HUD/2.png"), &dmnU1);
		break;
	case 3:
		ptrU1 = CargaImagen(TEXT("./HUD/3.png"), &dmnU1);
		break;
	case 4:
		ptrU1 = CargaImagen(TEXT("./HUD/4.png"), &dmnU1);
		break;
	case 5:
		ptrU1 = CargaImagen(TEXT("./HUD/5.png"), &dmnU1);
		break;
	case 6:
		ptrU1 = CargaImagen(TEXT("./HUD/6.png"), &dmnU1);
		break;
	case 7:
		ptrU1 = CargaImagen(TEXT("./HUD/7.png"), &dmnU1);
		break;
	case 8:
		ptrU1 = CargaImagen(TEXT("./HUD/8.png"), &dmnU1);
		break;
	case 9:
		ptrU1 = CargaImagen(TEXT("./HUD/9.png"), &dmnU1);
		break;
	default:
		ptrU1 = CargaImagen(TEXT("./HUD/0.png"), &dmnU1);
		break;
	}

	switch (u2)
	{
	case 0:
		ptrU2 = CargaImagen(TEXT("./HUD/0.png"), &dmnU2);
		break;
	case 1:
		ptrU2 = CargaImagen(TEXT("./HUD/1.png"), &dmnU2);
		break;
	case 2:
		ptrU2 = CargaImagen(TEXT("./HUD/2.png"), &dmnU2);
		break;
	case 3:
		ptrU2 = CargaImagen(TEXT("./HUD/3.png"), &dmnU2);
		break;
	case 4:
		ptrU2 = CargaImagen(TEXT("./HUD/4.png"), &dmnU2);
		break;
	case 5:
		ptrU2 = CargaImagen(TEXT("./HUD/5.png"), &dmnU2);
		break;
	case 6:
		ptrU2 = CargaImagen(TEXT("./HUD/6.png"), &dmnU2);
		break;
	case 7:
		ptrU2 = CargaImagen(TEXT("./HUD/7.png"), &dmnU2);
		break;
	case 8:
		ptrU2 = CargaImagen(TEXT("./HUD/8.png"), &dmnU2);
		break;
	case 9:
		ptrU2 = CargaImagen(TEXT("./HUD/9.png"), &dmnU2);
		break;
	default:
		ptrU2 = CargaImagen(TEXT("./HUD/0.png"), &dmnU2);
		break;
	}

	switch (d1)
	{
	case 0:
		ptrD1 = CargaImagen(TEXT("./HUD/0.png"), &dmnD1);
		break;
	case 1:
		ptrD1 = CargaImagen(TEXT("./HUD/1.png"), &dmnD1);
		break;
	case 2:
		ptrD1 = CargaImagen(TEXT("./HUD/2.png"), &dmnD1);
		break;
	case 3:
		ptrD1 = CargaImagen(TEXT("./HUD/3.png"), &dmnD1);
		break;
	case 4:
		ptrD1 = CargaImagen(TEXT("./HUD/4.png"), &dmnD1);
		break;
	case 5:
		ptrD1 = CargaImagen(TEXT("./HUD/5.png"), &dmnD1);
		break;
	case 6:
		ptrD1 = CargaImagen(TEXT("./HUD/6.png"), &dmnD1);
		break;
	case 7:
		ptrD1 = CargaImagen(TEXT("./HUD/7.png"), &dmnD1);
		break;
	case 8:
		ptrD1 = CargaImagen(TEXT("./HUD/8.png"), &dmnD1);
		break;
	case 9:
		ptrD1 = CargaImagen(TEXT("./HUD/9.png"), &dmnD1);
		break;
	default:
		ptrD1 = CargaImagen(TEXT("./HUD/0.png"), &dmnD1);
		break;
	}

	switch (d2)
	{
	case 0:
		ptrD2 = CargaImagen(TEXT("./HUD/0.png"), &dmnD2);
		break;
	case 1:
		ptrD2 = CargaImagen(TEXT("./HUD/1.png"), &dmnD2);
		break;
	case 2:
		ptrD2 = CargaImagen(TEXT("./HUD/2.png"), &dmnD2);
		break;
	case 3:
		ptrD2 = CargaImagen(TEXT("./HUD/3.png"), &dmnD2);
		break;
	case 4:
		ptrD2 = CargaImagen(TEXT("./HUD/4.png"), &dmnD2);
		break;
	case 5:
		ptrD2 = CargaImagen(TEXT("./HUD/5.png"), &dmnD2);
		break;
	case 6:
		ptrD2 = CargaImagen(TEXT("./HUD/6.png"), &dmnD2);
		break;
	case 7:
		ptrD2 = CargaImagen(TEXT("./HUD/7.png"), &dmnD2);
		break;
	case 8:
		ptrD2 = CargaImagen(TEXT("./HUD/8.png"), &dmnD2);
		break;
	case 9:
		ptrD2 = CargaImagen(TEXT("./HUD/9.png"), &dmnD2);
		break;
	default:
		ptrD2 = CargaImagen(TEXT("./HUD/0.png"), &dmnD2);
		break;
	}

	switch (c1)
	{
	case 0:
		ptrC1 = CargaImagen(TEXT("./HUD/0.png"), &dmnC1);
		break;
	case 1:
		ptrC1 = CargaImagen(TEXT("./HUD/1.png"), &dmnC1);
		break;
	case 2:
		ptrC1 = CargaImagen(TEXT("./HUD/2.png"), &dmnC1);
		break;
	case 3:
		ptrC1 = CargaImagen(TEXT("./HUD/3.png"), &dmnC1);
		break;
	case 4:
		ptrC1 = CargaImagen(TEXT("./HUD/4.png"), &dmnC1);
		break;
	case 5:
		ptrC1 = CargaImagen(TEXT("./HUD/5.png"), &dmnC1);
		break;
	case 6:
		ptrC1 = CargaImagen(TEXT("./HUD/6.png"), &dmnC1);
		break;
	case 7:
		ptrC1 = CargaImagen(TEXT("./HUD/7.png"), &dmnC1);
		break;
	case 8:
		ptrC1 = CargaImagen(TEXT("./HUD/8.png"), &dmnC1);
		break;
	case 9:
		ptrC1 = CargaImagen(TEXT("./HUD/9.png"), &dmnC1);
		break;
	default:
		ptrC1 = CargaImagen(TEXT("./HUD/0.png"), &dmnC1);
		break;
	}

	switch (c2)
	{
	case 0:
		ptrC2 = CargaImagen(TEXT("./HUD/0.png"), &dmnC2);
		break;
	case 1:
		ptrC2 = CargaImagen(TEXT("./HUD/1.png"), &dmnC2);
		break;
	case 2:
		ptrC2 = CargaImagen(TEXT("./HUD/2.png"), &dmnC2);
		break;
	case 3:
		ptrC2 = CargaImagen(TEXT("./HUD/3.png"), &dmnC2);
		break;
	case 4:
		ptrC2 = CargaImagen(TEXT("./HUD/4.png"), &dmnC2);
		break;
	case 5:
		ptrC2 = CargaImagen(TEXT("./HUD/5.png"), &dmnC2);
		break;
	case 6:
		ptrC2 = CargaImagen(TEXT("./HUD/6.png"), &dmnC2);
		break;
	case 7:
		ptrC2 = CargaImagen(TEXT("./HUD/7.png"), &dmnC2);
		break;
	case 8:
		ptrC2 = CargaImagen(TEXT("./HUD/8.png"), &dmnC2);
		break;
	case 9:
		ptrC2 = CargaImagen(TEXT("./HUD/9.png"), &dmnC2);
		break;
	default:
		ptrC2 = CargaImagen(TEXT("./HUD/0.png"), &dmnC2);
		break;
	}

	switch (m1)
	{
	case 0:
		ptrM1 = CargaImagen(TEXT("./HUD/0.png"), &dmnM1);
		break;
	case 1:
		ptrM1 = CargaImagen(TEXT("./HUD/1.png"), &dmnM1);
		break;
	case 2:
		ptrM1 = CargaImagen(TEXT("./HUD/2.png"), &dmnM1);
		break;
	case 3:
		ptrM1 = CargaImagen(TEXT("./HUD/3.png"), &dmnM1);
		break;
	case 4:
		ptrM1 = CargaImagen(TEXT("./HUD/4.png"), &dmnM1);
		break;
	case 5:
		ptrM1 = CargaImagen(TEXT("./HUD/5.png"), &dmnM1);
		break;
	case 6:
		ptrM1 = CargaImagen(TEXT("./HUD/6.png"), &dmnM1);
		break;
	case 7:
		ptrM1 = CargaImagen(TEXT("./HUD/7.png"), &dmnM1);
		break;
	case 8:
		ptrM1 = CargaImagen(TEXT("./HUD/8.png"), &dmnM1);
		break;
	case 9:
		ptrM1 = CargaImagen(TEXT("./HUD/9.png"), &dmnM1);
		break;
	default:
		ptrM1 = CargaImagen(TEXT("./HUD/0.png"), &dmnM1);
		break;
	}

	switch (m1)
	{
	case 0:
		ptrM2 = CargaImagen(TEXT("./HUD/0.png"), &dmnM2);
		break;
	case 1:
		ptrM2 = CargaImagen(TEXT("./HUD/1.png"), &dmnM2);
		break;
	case 2:
		ptrM2 = CargaImagen(TEXT("./HUD/2.png"), &dmnM2);
		break;
	case 3:
		ptrM2 = CargaImagen(TEXT("./HUD/3.png"), &dmnM2);
		break;
	case 4:
		ptrM2 = CargaImagen(TEXT("./HUD/4.png"), &dmnM2);
		break;
	case 5:
		ptrM2 = CargaImagen(TEXT("./HUD/5.png"), &dmnM2);
		break;
	case 6:
		ptrM2 = CargaImagen(TEXT("./HUD/6.png"), &dmnM2);
		break;
	case 7:
		ptrM2 = CargaImagen(TEXT("./HUD/7.png"), &dmnM2);
		break;
	case 8:
		ptrM2 = CargaImagen(TEXT("./HUD/8.png"), &dmnM2);
		break;
	case 9:
		ptrM2 = CargaImagen(TEXT("./HUD/9.png"), &dmnM2);
		break;
	default:
		ptrM2 = CargaImagen(TEXT("./HUD/0.png"), &dmnM2);
		break;
	}

}

void DibujarMiles1(int* buffer, int* hud, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO - 1;
	int posX = pos.X;
	int posY = pos.Y;

	__asm
	{
		cld
		mov esi, hud
		mov edi, buffer

		mov eax, posX
		mul BPP
		add edi, eax
		mov eax, ANCHO_VENTANA
		mul BPP
		mul posY
		add edi, eax

		mov eax, w
		mul BPP
		add esi, eax

		xor ecx, ecx
		mov ecx, h

		repite2 :

		push ecx
			mov ecx, w


			mostrar2 :
		mov eax, [esi]
			cmp eax, 0FFFF0000h
			je Color2
			mov[edi], eax


			Color2 :

		add esi, BPP
			add edi, BPP


			loop mostrar2
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			MOV EAX, w
			MUL BPP
			SUB EDI, EAX

			mov eax, w
			mul BPP

			add esi, eax
			mov eax, w
			mul BPP
			sub esi, eax



			pop ecx

			loop repite2
	}
}

void DibujarMiles2(int* buffer, int* hud, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO - 1;
	int posX = pos.X;
	int posY = pos.Y;

	__asm
	{
		cld
		mov esi, hud
		mov edi, buffer

		mov eax, posX
		mul BPP
		add edi, eax
		mov eax, ANCHO_VENTANA
		mul BPP
		mul posY
		add edi, eax

		mov eax, w
		mul BPP
		add esi, eax

		xor ecx, ecx
		mov ecx, h

		repite2 :

		push ecx
			mov ecx, w


			mostrar2 :
		mov eax, [esi]
			cmp eax, 0FFFF0000h
			je Color2
			mov[edi], eax


			Color2 :

		add esi, BPP
			add edi, BPP


			loop mostrar2
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			MOV EAX, w
			MUL BPP
			SUB EDI, EAX

			mov eax, w
			mul BPP

			add esi, eax
			mov eax, w
			mul BPP
			sub esi, eax



			pop ecx

			loop repite2
	}
}

void DibujarCentenas1(int* buffer, int* hud, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO - 1;
	int posX = pos.X;
	int posY = pos.Y;

	__asm
	{
		cld
		mov esi, hud
		mov edi, buffer

		mov eax, posX
		mul BPP
		add edi, eax
		mov eax, ANCHO_VENTANA
		mul BPP
		mul posY
		add edi, eax

		mov eax, w
		mul BPP
		add esi, eax

		xor ecx, ecx
		mov ecx, h

		repite2 :

		push ecx
			mov ecx, w


			mostrar2 :
		mov eax, [esi]
			cmp eax, 0FFFF0000h
			je Color2
			mov[edi], eax


			Color2 :

		add esi, BPP
			add edi, BPP


			loop mostrar2
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			MOV EAX, w
			MUL BPP
			SUB EDI, EAX

			mov eax, w
			mul BPP

			add esi, eax
			mov eax, w
			mul BPP
			sub esi, eax



			pop ecx

			loop repite2
	}
}

void DibujarCentenas2(int* buffer, int* hud, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO - 1;
	int posX = pos.X;
	int posY = pos.Y;

	__asm
	{
		cld
		mov esi, hud
		mov edi, buffer

		mov eax, posX
		mul BPP
		add edi, eax
		mov eax, ANCHO_VENTANA
		mul BPP
		mul posY
		add edi, eax

		mov eax, w
		mul BPP
		add esi, eax

		xor ecx, ecx
		mov ecx, h

		repite2 :

		push ecx
			mov ecx, w


			mostrar2 :
		mov eax, [esi]
			cmp eax, 0FFFF0000h
			je Color2
			mov[edi], eax


			Color2 :

		add esi, BPP
			add edi, BPP


			loop mostrar2
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			MOV EAX, w
			MUL BPP
			SUB EDI, EAX

			mov eax, w
			mul BPP

			add esi, eax
			mov eax, w
			mul BPP
			sub esi, eax



			pop ecx

			loop repite2
	}
}

void DibujarDecenas1(int* buffer, int* hud, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO - 1;
	int posX = pos.X;
	int posY = pos.Y;

	__asm
	{
		cld
		mov esi, hud
		mov edi, buffer

		mov eax, posX
		mul BPP
		add edi, eax
		mov eax, ANCHO_VENTANA
		mul BPP
		mul posY
		add edi, eax

		mov eax, w
		mul BPP
		add esi, eax

		xor ecx, ecx
		mov ecx, h

		repite2 :

		push ecx
			mov ecx, w


			mostrar2 :
		mov eax, [esi]
			cmp eax, 0FFFF0000h
			je Color2
			mov[edi], eax


			Color2 :

		add esi, BPP
			add edi, BPP


			loop mostrar2
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			MOV EAX, w
			MUL BPP
			SUB EDI, EAX

			mov eax, w
			mul BPP

			add esi, eax
			mov eax, w
			mul BPP
			sub esi, eax



			pop ecx

			loop repite2
	}
}

void DibujarDecenas2(int* buffer, int* hud, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO - 1;
	int posX = pos.X;
	int posY = pos.Y;

	__asm
	{
		cld
		mov esi, hud
		mov edi, buffer

		mov eax, posX
		mul BPP
		add edi, eax
		mov eax, ANCHO_VENTANA
		mul BPP
		mul posY
		add edi, eax

		mov eax, w
		mul BPP
		add esi, eax

		xor ecx, ecx
		mov ecx, h

		repite2 :

		push ecx
			mov ecx, w


			mostrar2 :
		mov eax, [esi]
			cmp eax, 0FFFF0000h
			je Color2
			mov[edi], eax


			Color2 :

		add esi, BPP
			add edi, BPP


			loop mostrar2
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			MOV EAX, w
			MUL BPP
			SUB EDI, EAX

			mov eax, w
			mul BPP

			add esi, eax
			mov eax, w
			mul BPP
			sub esi, eax



			pop ecx

			loop repite2
	}
}

void DibujarUnidades1(int* buffer, int* hud, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO - 1;
	int posX = pos.X;
	int posY = pos.Y;

	__asm
	{
		cld
		mov esi, hud
		mov edi, buffer

		mov eax, posX
		mul BPP
		add edi, eax
		mov eax, ANCHO_VENTANA
		mul BPP
		mul posY
		add edi, eax

		mov eax, w
		mul BPP
		add esi, eax

		xor ecx, ecx
		mov ecx, h

		repite2 :

		push ecx
			mov ecx, w


			mostrar2 :
		mov eax, [esi]
			cmp eax, 0FFFF0000h
			je Color2
			mov[edi], eax


			Color2 :

		add esi, BPP
			add edi, BPP


			loop mostrar2
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			MOV EAX, w
			MUL BPP
			SUB EDI, EAX

			mov eax, w
			mul BPP

			add esi, eax
			mov eax, w
			mul BPP
			sub esi, eax



			pop ecx

			loop repite2
	}
}

void DibujarUnidades2(int* buffer, int* hud, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO - 1;
	int posX = pos.X;
	int posY = pos.Y;

	__asm
	{
		cld
		mov esi, hud
		mov edi, buffer

		mov eax, posX
		mul BPP
		add edi, eax
		mov eax, ANCHO_VENTANA
		mul BPP
		mul posY
		add edi, eax

		mov eax, w
		mul BPP
		add esi, eax

		xor ecx, ecx
		mov ecx, h

		repite2 :

		push ecx
			mov ecx, w


			mostrar2 :
		mov eax, [esi]
			cmp eax, 0FFFF0000h
			je Color2
			mov[edi], eax


			Color2 :

		add esi, BPP
			add edi, BPP


			loop mostrar2
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			MOV EAX, w
			MUL BPP
			SUB EDI, EAX

			mov eax, w
			mul BPP

			add esi, eax
			mov eax, w
			mul BPP
			sub esi, eax



			pop ecx

			loop repite2
	}
}

#pragma endregion



