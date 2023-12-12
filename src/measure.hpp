#ifndef measure_hpp
#define measure_hpp
void displayADC(int, int, double) ;
void voltsRange(int);
void ampsRange(int);
void ohmsRange(int);
double voltsMeas(int, double);
void ampsMeas(int, double);
void ohmsMeas(int, double);  //continuity included?
void displayPostfix(int, int);

#endif