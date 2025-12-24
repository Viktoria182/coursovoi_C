#include<stdio.h>
#include<math.h>
#include<float.h>
#include<locale.h>

#define H 0.0001
#define DERIVATIVE(f,x) (((f)((x)+H)-(f)((x)-H))/(2*H))

double f(double x);
double derivative(double x);
double find_min_max(double start, double finish, double step);  
double find_x_for_y(double y, double prec, double start, double finish);
void save_table_to_file(double start, double finish, double step);
void print_table(double start, double finish, double step);


int main() {
	setlocale(LC_ALL, "RUS");
	char w = 'y';
	while (w == 'y') {

		printf("Меню возможных операций.\n");
		printf("1) Значение функции в точке.\n");
		printf("2) Таблица - x->f(x) на интервале.\n");
		printf("3) Min/Max - экстремумы на отрезке.\n");
		printf("4) Поиск x - найти x: f(x) = Y.\n");
		printf("5) Производная - f'(x) в точке.\n");
		printf("6) Сохранить таблицу в файл.\n");
		printf("!!!При x=0 в графике функции разрыв, необходимо это учесть при вводе значений.!!!\n");
		printf("Введите номер операции: ");

		int input;
		scanf("%d", &input);

		switch (input) {
		case 1: {
			double x;

			printf("Введите x: ");
			scanf("%lf", &x);
			printf("f(%lf) = %lf", x, f(x));

			break;
		}
		case 2: {
			double start, finish, step;

			printf("Введите начало интервала: ");
			scanf("%lf", &start);
			printf("Введите конец интервала: ");
			scanf("%lf", &finish);
			printf("Введите шаг: ");
			scanf("%lf", &step);
			if (step <= 0) {
				printf("Ошибка: шаг должен быть положительным!\n");
				break;
			}
			print_table(start, finish, step);
			break;
		}
		case 3: {
			double start, finish, step;

			printf("Введите начало отрезка: ");
			scanf("%lf", &start);
			printf("Введите конец отрезка: ");
			scanf("%lf", &finish);
			printf("Введите шаг поиска: ");
			scanf("%lf", &step);
			
			if (step <= 0) {
				printf("Ошибка: шаг должен быть положительным!\n");
				break;
			}

			double extreme_point = find_min_max(start, finish, step);
			if (extreme_point >= start && extreme_point <= finish) {
				printf("Найдена точка экстремума: x = %.6lf, f(x) = %.6lf\n", extreme_point, f(extreme_point));
			}
			else {
				printf("Экстремумы не найдены на заданном отрезке.\n");
			}
			break;
		}
		case 4: {
			double y, prec, start, finish;

			printf("Введите Y: ");
			scanf("%lf", &y);
			printf("Введите точность: ");
			scanf("%lf", &prec);
			printf("Введите начало интервала поиска: ");
			scanf("%lf", &start);
			printf("Введите конец интервала поиска: ");
			scanf("%lf", &finish);
			double x_result = find_x_for_y(y, prec, start, finish);
			printf("x = %.6lf, f(x) = %.6lf\n", x_result, f(x_result));
			break;
		}
		case 5: {
			double x;

			printf("Введите x: ");
			scanf("%lf", &x);

			printf("f'(%lf) = %f\n", x, derivative(x));
			break;
		}
		case 6: {
			double start, finish, step;
			printf("Введите начало интервала: ");
			scanf("%lf", &start);
			printf("Введите конец интервала: ");
			scanf("%lf", &finish);
			printf("Введите шаг: ");
			scanf("%lf", &step);

			if (step <= 0) {
				printf("Ошибка: шаг должен быть положительным!\n");
				break;
			}
			save_table_to_file(start, finish, step);
			break;
		}
		default:
			printf("Операция не найдена!");
		}
		while (getchar() != '\n');
		printf("\nЕсли хотите продолжить работу с калькулятором нажмите 'y': ");
		scanf("%c", &w);
	}
	return 0;
}

double f(double x) {
	if (x < 0) {
		return x*x + 1;
	}
	else if (x >= 3) {
		double res = 0;
		for (int n = 0; n <= 13; n++) {
			double a = (pow(-1, n) * pow(x, 2 * n + 1));
			double b = (tgamma(2 * n + 1) * (n + 1));
			res += a / b;
		}
		return res;
	}
	else {
		return ((exp(sin(x)) * log(1 + fabs(x))) / (sqrt(pow(x, 2) + 1))) + atan(pow(x, 3));
	}
}

double derivative(double x) {
	return DERIVATIVE(f, x);
}

double find_min_max(double start, double finish, double step) {
	if (step <= 0 || start > finish) {
		return start - 1;
	}
	double best_x = start;
	double min_deriv = 1e9;
	double prev_deriv = derivative(start);

	for (double x = start + step; x <= finish + step/2; x += step) {
		if (x > finish) break;

		double current_deriv = derivative(x);

		if (prev_deriv * current_deriv <=0) {
			return (x - step / 2);
		}
		double deriv_abs = fabs(current_deriv);
		if (deriv_abs < min_deriv) {
			min_deriv = deriv_abs;
			best_x = x;
		}
		prev_deriv = current_deriv;
	}
	return best_x;

}

double find_x_for_y(double y, double prec, double start, double finish) {



	for (double x = start; x <= finish; x += prec) {
		if (fabs(f(x) - y) < prec) {
			return x;

			break;
		}
	}
}
void save_table_to_file(double start, double finish, double step) {
	char filename[100];
	printf("Введите имя файла для сохранения (например: table.txt): ");
	scanf("%s", filename);

	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("Ошибка: не удалось создать файл '%s'\n", filename);
		return;
	}

	fprintf(file,"|     x     |    f(x)  |\n");
	fprintf(file,"|___________|__________|\n");

	double x = start;

	while (x <= finish + step / 2 && x<= finish) {
		double fx = f(x);
		fprintf(file,"|%10.4lf |%10.4lf|\n", x, fx);
		x += step;
	}
	fclose(file);
	printf("Таблица успешно сохранена в файл '%s'\n", filename);
}
void print_table(double start, double finish, double step) {
	printf("|     x     |    f(x)  |\n");
	printf("|___________|__________|\n");

	for (double x = start; x <= finish+step/2; x += step) {
		double fx = f(x);
		if (x >= start && x <= finish) {
			printf("|%10.4lf |%10.4lf|\n", x, fx);
		}
	}
}