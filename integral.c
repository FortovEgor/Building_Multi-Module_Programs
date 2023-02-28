// ФОРТОВ ЕГОР, БПИ212
// ВАРИАНТ: 5 4 2
#include <stdio.h>
#include <math.h>
#include <string.h>

#define F(f, g, a) (f(a) - g(a))  // F(x) = f(x) - g(x)
typedef double afunc(double);
long long unsigned num_of_iterations = 0; // количество итераций для вычисления абсцисс точек пересечени графиков

extern double f1(double);
extern double f2(double);
extern double f3(double);
extern double f1_div_1(double);
extern double f2_div_1(double);
extern double f3_div_1(double);
extern double f1_div_2(double);
extern double f2_div_2(double);
extern double f3_div_2(double);

// ФУНКЦИИ ----------- НАЧАЛО -----------
/*
double f1(double x) {
    return 0.35 * x * x - 0.95 * x + 2.7;
}

double f1_div_1(double x) {
    return 0.7 * x - 0.95;
}

double f1_div_2(double x) {
    return 0.7;
}
*/
/*
double f2(double x) {
    return 3 * x + 1;
}

double f2_div_1(double x) {
    return 3;
}

double f2_div_2(double x) {
    return 0;
}
*/
/*
double f3(double x) {
    return 1 / (x + 2);
}
double f3_div_1(double x) {
    return -1 / ((x + 2) * (x + 2));
}

double f3_div_2(double x) {
    return 2 / ((x+2) * (x+2) * (x+2));
}
*/

// ФУНКЦИИ ----------- КОНЕЦ -----------


//функция, которая считает интеграл от функции f(x) с помощью формулы прямоугольников
static double integral(afunc *f, double a, double b, double eps){
    int n = 10;  // начальное количество трапеций
    double I_n;  // сумма площадей n трапеций
    double I_2n;  // сумма площадей 2n трапеций
    double p = 1 / (double) 3;  // для правила Рунге

    do {
        I_n = 0;
        I_2n = 0;
        double h = (b - a)/(double)n;
        for(int i = 0; i <= n; i++){
            // применяем формулу трапеций (в зависимости от i)
            if (i == 0) {
                I_n += h*(f(a + i*h)) * 0.5;
                continue;
            } else if (i == n) {
                I_n += h*(f(a + i*h)) * 0.5;
                continue;
            }
            I_n += h*(f(a + i*h));
        }

        h /= (double) 2;
        for (int i = 0; i <= 2*n; i++) {
            // применяем формулу трапеций (в зависимости от i)
            if (i == 0) {
                I_2n += h*(f(a + i*h)) * 0.5;
                continue;
            } else if (i == 2*n) {
                I_2n += h*(f(a + i*h)) * 0.5;
                continue;
            }
            I_2n += h*(f(a + i*h));
        }

        n *= 2;  // увеличиваем число трапеций (для повышения точности)
    } while(p*fabs(I_n - I_2n) > eps); //применяем правило Рунге

    return I_2n;
}

static double root(afunc *f, afunc *f_div_1, afunc *f_div_2, afunc *g, afunc *g_div_1, afunc *g_div_2, double a, double b, double eps1) {
    // функция находит абсциссу точки пересечения функции F(x) = f(x) - g(x)
    // с осью OX с помощью комбинированного метода (хорд и касательных)
    // F(x) = f(x) - g(x)
    // F'(x) = f_div_1(x) - g_div_1(x)
    // F''(x) = f_div_2(x) - g_div_2(x)
    do {
        if (F(f, g, a) * F(f_div_2, g_div_2, a) < 0) { // Условие начальной точки a для метода хорд
            a += (b - a) / (F(f, g, a) - F(f, g, b)) * F(f, g, a);
        } else if (F(f, g, a) * F(f_div_2, g_div_2, a) > 0) {  // Условие начальной точки a для метода касательных
            a -= F(f, g, a) / F(f_div_1, g_div_1, a);
        }

        if (F(f, g, b) * F(f_div_2, g_div_2, b) < 0) { // Условие начальной точки b для метода хорд
            b += (a - b) / (F(f, g, b) - F(f, g, a)) * F(f, g, b);
        } else if (F(f, g, b) * F(f_div_2, g_div_2, b) > 0) {  // Условие начальной точки b для метода касательных
            b -= F(f, g, b) / F(f_div_1, g_div_1, b);
        }

        num_of_iterations++;
    } while (fabs(a - b) > 2 * eps1);  // Построение хорд и касательных продолжается до достижения необходимой точности решения е
    return (a + b) / 2.0;
}

static void get_function(int number, afunc *fp[]) {
    switch(number) {
        case 1:
            fp[0] = f1;
            fp[1] = f1_div_1;
            fp[2] = f1_div_2;
            return;
        case 2:
            fp[0] = f2;
            fp[1] = f2_div_1;
            fp[2] = f2_div_2;
            return;
        case 3:
            fp[0] = f3;
            fp[1] = f3_div_1;
            fp[2] = f3_div_2;
            return;
        default:
            return;
    }
}


int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--help") || (!strcmp(argv[i], "-h"))) {
            printf("Справка по командам:\n");
            printf("--root -r  абсциссы точек пересечения кривых (см. ниже)\n");
            printf("--iterations -i число итераций при подсчете абсцисс точек пересечения графиков функций (см. ниже)\n");
            printf("--test-root -R тест функции root; параметры вызова root имеют вид F1:F2:A:B:E:R\n");
            printf("F1, F2 - номера используемых функций\nA, B, E - значения параметров a, b, eps1 функции root\nR - правильный ответ\n");
            printf("--test-integral -I тест функции integral; параметры вызова integral имеют вид F:A:B:E:R\n");
            printf("F - номер используемой функции\nA, B, E - значения параметров a, b, eps2 функции integral\nR - правильный ответ\n");
            printf("--help -h  показать эту справку и выйти\n\n");
            printf("Функции:\n");
            printf("f1: 0.35 * x * x - 0.95 * x + 2.7\nf2: 3 * x + 1\nf3: 1 / (x + 2)\n");
            return 0;
        }
        if (!strcmp(argv[i], "--root") || (!strcmp(argv[i], "-r"))) {
            // eps = 0.0001
            double eps = 0.000001;  // задаем такую точность вычислений чтобы гарантированно "уложиться" в итоговую точность в 0.0001
            double x13 = root(f1, f1_div_1, f1_div_2, f3, f3_div_1, f3_div_2, -1.9, -1.8, eps);  // абсцисса точки пересечения графиков f1 и f3
            double x12 = root(f1, f1_div_1, f1_div_2, f2, f2_div_1, f2_div_2, 0.4, 0.5, eps);  // абсцисса точки пересечения графиков f1 и f3
            double x23 = root(f2, f2_div_1, f2_div_2, f3, f3_div_1, f3_div_2, -0.2, 0, eps);  // абсцисса точки пересечения графиков f2 и f3
            printf("Функции:\n");
            printf("f1: 0.35 * x * x - 0.95 * x + 2.7\nf2: 3 * x + 1\nf3: 1 / (x + 2)\n");
            printf("Абсциссы точек пересечения этих кривых:\n");
            printf("x12: %lf; x13: %lf; x23: %lf;\n", x12, x13, x23);

        }
        if (!strcmp(argv[i], "--iterations") || (!strcmp(argv[i], "-i"))) {

            printf("Функции:\n");
            printf("f1: 0.35 * x * x - 0.95 * x + 2.7\nf2: 3 * x + 1\nf3: 1 / (x + 2)\n");
            printf("Количество итераций при подсчете абсциссы точки пересечения функций f1 и f3: %lld\n", num_of_iterations);
            num_of_iterations = 0;
            printf("Количество итераций при подсчете абсциссы точки пересечения функций f1 и f2: %lld\n", num_of_iterations);
            num_of_iterations = 0;
            printf("Количество итераций при подсчете абсциссы точки пересечения функций f1 и f3: %lld\n", num_of_iterations);
        }
        // root
        if (!strcmp(argv[i], "--test-root") || (!strcmp(argv[i], "-R"))) {
            if (i+1 >= argc) {
                fprintf(stderr, "missing arguement value for -R;\nexample: ./%s -R 1:2:0.4:0.5:0.000001:0.448178\n", argv[0]);
                return 0;
            }
            int func_1, func_2;
            double x_l, x_r, epsilon, correct_answer;
            if(sscanf(argv[i+1], "%d:%d:%lf:%lf:%lf:%lf", &func_1, &func_2, &x_l, &x_r, &epsilon, &correct_answer) != 6) {
                fprintf(stderr, "wrong format of arguements for -R\n");
                return 0;
            }
            afunc *fp_1[3], *fp_2[3];
            get_function(func_1, fp_1);
            get_function(func_2, fp_2);
            double got_answer = root(fp_1[0], fp_1[1], fp_1[2], fp_2[0], fp_2[1], fp_2[2], x_l, x_r, epsilon);
            printf("Функции:\n");
            printf("f1: 0.35 * x * x - 0.95 * x + 2.7\nf2: 3 * x + 1\nf3: 1 / (x + 2)\n");
            printf("Результат вычислений:%lf\nАбсолютная погрешность:%lf\nОтносительная погрешность:%lf\n", got_answer, fabs(got_answer-correct_answer), fabs(got_answer-correct_answer)/got_answer);
        }

        // integral
        if (!strcmp(argv[i], "--test-integral") || (!strcmp(argv[i], "-I"))) {
            if (i+1 >= argc) {
                fprintf(stderr, "missing arguement value for -R;\nexample: ./%s -R 1:2:0.4:0.5:0.000001\n", argv[0]);
                return 0;
            }
            int func;
            double x_l, x_r, epsilon, correct_answer;
            if(sscanf(argv[i+1], "%d:%lf:%lf:%lf:%lf", &func, &x_l, &x_r, &epsilon, &correct_answer) != 5) {
                fprintf(stderr, "wrong format of arguements for -R\n");
                return 0;
            }
            afunc *fp[3];
            get_function(func, fp);
            double got_answer = integral(fp[0], x_l, x_r, epsilon);
            printf("Функции:\n");
            printf("f1: 0.35 * x * x - 0.95 * x + 2.7\nf2: 3 * x + 1\nf3: 1 / (x + 2)\n");
            printf("Результат вычислений:%lf\nАбсолютная погрешность:%lf\nОтносительная погрешность:%lf\n", got_answer, fabs(got_answer-correct_answer), fabs(got_answer-correct_answer)/got_answer);
        }
    }
    // eps = 0.0001
    double eps = 0.000001;  // задаем такую точность вычислений чтобы гарантированно "уложиться" в итоговую точность в 0.0001
    double x13 = root(f1, f1_div_1, f1_div_2, f3, f3_div_1, f3_div_2, -1.9, -1.8, eps);  // абсцисса точки пересечения графиков f1 и f3
    double x12 = root(f1, f1_div_1, f1_div_2, f2, f2_div_1, f2_div_2, 0.4, 0.5, eps);  // абсцисса точки пересечения графиков f1 и f3
    double x23 = root(f2, f2_div_1, f2_div_2, f3, f3_div_1, f3_div_2, -0.2, 0, eps);  // абсцисса точки пересечения графиков f2 и f3

    double figure_1 = integral(f1, x13, x12, eps);  // парабола (f1)
    // printf("|%lf %lf %lf|", x13, x12, figure_1);  // for DEBUG
    double figure_2 = integral(f2, x23, x12, eps);  // линейная функция (f2)
    double figure_3 = integral(f3, x13, x23, eps);  // гипербола (f3)

    double square = figure_1 - figure_2 - figure_3;
    printf("\nTOTAL MAIN ANSWER:%lf\n", square);
    return 0;
}
