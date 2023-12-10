#ifndef measure_hpp
#define measure_hpp
void displayADC(int config, int config2, double offset);
void voltsRange(int);
void ampsRange(int config2);
void ohmsRange(int config2);
double voltsMeas(int config2, double offset);
void ampsMeas(int config2, double offset);
void ohmsMeas(int config2, double offset);  //continuity included?
void displayPostfix(int config, int config2);

#endif

