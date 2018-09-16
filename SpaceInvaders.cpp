#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
#include<list>
using namespace std;

void gotoxy(int x, int y){
	HANDLE hCon;
	hCon=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X=x;
	dwPos.Y=y;
	
	SetConsoleCursorPosition(hCon, dwPos);
}

void ocultarcursor(){
	HANDLE hCon;
	hCon=GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize=2;
	cci.bVisible=FALSE;
	
	SetConsoleCursorInfo(hCon,&cci);
}

void pintar_limites(){
	for(int i=2;i<78;i++){
		gotoxy(i,3); printf("%c",205);
		gotoxy(i,33); printf("%c",205);
	}
	for(int i=4;i<33;i++){
		gotoxy(2,i); printf("%c",186);
		gotoxy(77,i); printf("%c",186);
	}
	gotoxy(2,3); printf("%c",201);
	gotoxy(2,33); printf("%c",200);
	gotoxy(77,3); printf("%c",187);
	gotoxy(77,33); printf("%c",188);
}

class Nave{
	private:
		int x,y,salud,vidas;
	public:
		Nave(int,int,int,int);
		int X(){return x;}
		int Y(){return y;}
		void pintar();
		void borrar();
		void mover();
		void cant_salud();
		void explosion();
		void cor(){salud--;}
		int V(){return vidas;}
};

Nave::Nave(int _x,int _y,int _salud,int _vidas){
	x=_x;
	y=_y;
	salud=_salud;
	vidas=_vidas;
}

void Nave::pintar(){
	gotoxy(x,y); printf("  %c",186);
	gotoxy(x,y+1); printf(" %c%c%c",91,254,93);
	gotoxy(x,y+2); printf("%c%c %c%c",60,219,219,62);
}

void Nave::borrar(){
	gotoxy(x,y); printf("        ");
	gotoxy(x,y+1); printf("        ");
	gotoxy(x,y+2); printf("        ");
}

void Nave::mover(){
	if(kbhit()){
			char tecla=getch();
			borrar();
			if(tecla==IZQUIERDA&&x>3) x--;
			if(tecla==DERECHA&&x+6<77) x++;
			if(tecla==ARRIBA&&y>4) y--;
			if(tecla==ABAJO&&y+3<33) y++;
			if(tecla=='e')salud--;
			pintar();
			cant_salud();
			explosion();
		}
}

void Nave::cant_salud(){
	gotoxy(50,2); printf("Vidas: %d",vidas);
	gotoxy(64,2); printf("Salud");
	gotoxy(70,2); printf("      ");
	for(int i=0;i<salud;i++){
		gotoxy(70+i,2); printf("%c",206);
	}
}

void Nave::explosion(){
	if(salud==0){
		borrar();
		gotoxy(x,y);   printf("   **   ");
		gotoxy(x,y+1); printf("  ****  ");
		gotoxy(x,y+2); printf("   **  ");
		Sleep(200);
		borrar();
		gotoxy(x,y);   printf(" * ** *");
		gotoxy(x,y+1); printf("  **** ");
		gotoxy(x,y+2); printf(" * ** *");
		Sleep(200);
		borrar();
	
		vidas--;
		salud=3;
		cant_salud();
		pintar();
	}
}

class Asteroide{
	private:
		int x,y;
	public:
		Asteroide(int _x,int _y):x(_x),y(_y){}
		void pintar();
		void mover();
		void borrar();
		void choque(class Nave &N);
		int X(){return x;}
		int Y(){return y;}
};

void Asteroide::pintar(){
	gotoxy(x,y);printf("%c",184);
}

void Asteroide::mover(){
	gotoxy(x,y); printf(" ");
	y++;
	if(y>32){
		x=rand()%71+4;
		y=4;
	}
	pintar();
}

void Asteroide::borrar(){
	gotoxy(x,y); printf(" ");
}

void Asteroide::choque(class Nave &N){
	if(x>= N.X()-1&&x<N.X()+6&&y>=N.Y()-1&&y<=N.Y()+3){
		N.cor();
		N.borrar();
		borrar();
		N.pintar();
		N.cant_salud();
		x=rand()%71+4;
		y=4;
	}
}

class Bala{
	private:
		int x,y;
	public:
		Bala(int _x,int _y):x(_x),y(_y){}
		void mover();
		bool fuera();
		int X(){return x;}
		int Y(){return y;}
		
};

void Bala::mover(){
	gotoxy(x,y); printf(" ");
	y--;	
	gotoxy(x,y); printf("%c",39);
}

bool Bala::fuera(){
	if(y==4){
		return true;
	}
	else{
		return false;
	}
}

int main () {
	
	ocultarcursor();
	pintar_limites();
	Nave N(37,30,3,3);
	N.pintar();
	N.cant_salud();
	list<Asteroide *> A;
	list<Asteroide *>::iterator itA;
	
	for(int i=0;i<5;i++){
		A.push_back(new Asteroide(rand()%75+3,rand()%5+4 ));
	}
	
	list<Bala*>B;
	list<Bala*>::iterator it;
	
	bool game_over=false;
	int puntos=0;
	while(!game_over){
		gotoxy(4,2); printf("Puntos: %i",puntos);
		if(kbhit()){
			char tecla=getch();
	 		if(tecla=='a'){
				B.push_back(new Bala(N.X()+2,N.Y()-1));
			}
		}
		
		for(it=B.begin(); it!=B.end(); it++){
			(*it)->mover();
			if((*it)->fuera()){
				gotoxy((*it)->X(),(*it)->Y()); printf(" ");
				delete(*it);
				it=B.erase(it);
			}
		}
		
		for(itA=A.begin();itA!=A.end();itA++){
			(*itA)->mover();
			(*itA)->choque(N);
		}
		
		for(itA=A.begin();itA!=A.end();itA++){
			for(it=B.begin();it!=B.end();it++){
				if((*itA)->X()==(*it)->X()&& ((*itA)->Y()+1==(*it)->Y()||(*itA)->Y()==(*it)->Y())){
					gotoxy((*it)->X(),(*it)->Y()); printf(" ");
					delete (*it);
					it=B.erase(it);
					
					A.push_back(new Asteroide(rand()%74+3,4));
					gotoxy((*itA)->X(),(*itA)->Y()); printf(" ");
					delete(*itA);
					itA=A.erase(itA);
					
					puntos++;
				}

			}
		}
		if(N.V()==0){
			game_over=true;
		}
		N.mover();
		N.cant_salud();
		N.explosion();
		Sleep(30);
	}
	
	
	system("cls");
	printf("\n\t.:.:.Ha perdido, es usted burda de maleta.:.:.\n");
	
	return 0;
}