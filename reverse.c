#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_LINE_LENGTH 1024

// Función para obtener texto desde la consola y almacenarlo en un array de strings
char **obtener_texto_consola()
{
    char **lines = NULL;
    char buffer[MAX_LINE_LENGTH];
    int num_lines = 0;

    lines = (char **)malloc(sizeof(char *));
    if (lines == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    while (fgets(buffer, MAX_LINE_LENGTH, stdin) != NULL)
    {
        // Verificar si la línea es un salto de línea
        if (strcmp(buffer, "\n") == 0)
        {
            break;
        }

        lines[num_lines] = (char *)malloc(strlen(buffer) + 1);
        if (lines[num_lines] == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        strcpy(lines[num_lines], buffer);

        num_lines++;

        lines = (char **)realloc(lines, (num_lines + 1) * sizeof(char *));
        if (lines == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
    }
    lines[num_lines] = NULL;

    return lines;
}

// Función para cargar un archivo en un array de strings
char **cargar_archivo(const char *nombre_archivo)
{
    FILE *archivoDeEntrada;

    archivoDeEntrada = fopen(nombre_archivo, "r");
    if (archivoDeEntrada == NULL)
    {
        fprintf(stderr, "reverse: cannot open file '%s'\n", nombre_archivo);
        exit(1);
    }

    int numeroDeLineas = 0;

    // Almacenamiento temporal para cada línea leída
    char textoDeEntrada[MAX_LINE_LENGTH];

    // Cuenta el número de líneas
    while (fgets(textoDeEntrada, sizeof(textoDeEntrada), archivoDeEntrada))
    {
        numeroDeLineas++;
    }

    fseek(archivoDeEntrada, 0, SEEK_SET);

    char **textoAlmacenado = (char **)malloc(sizeof(char *) * (numeroDeLineas + 1));

    if (textoAlmacenado == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    // Lee archivoDeEntrada línea por línea y las almacena en los espacios de memoria reservados para cada línea
    for (int i = 0; i < numeroDeLineas; i++)
    {
        textoAlmacenado[i] = (char *)malloc(sizeof(char) * sizeof(textoDeEntrada));
        fgets(textoAlmacenado[i], MAX_LINE_LENGTH, archivoDeEntrada);
    }

    textoAlmacenado[numeroDeLineas] = NULL;

    fclose(archivoDeEntrada); // Cerrar el archivo después de leerlo completamente

    // Devuelve el arreglo de punteros a las líneas leídas
    return textoAlmacenado;
}

// Función para invertir el orden de un array de strings
void invertir_textos(char **textos)
{
    int length = 0;

    // Calcular la longitud del array
    while (textos[length] != NULL)
    {
        length++;
    }

    // Intercambiar elementos del array
    int i = 0, j = length - 1;
    while (i < j)
    {
        char *temp = textos[i];
        textos[i] = textos[j];
        textos[j] = temp;
        i++;
        j--;
    }
}

// Función para verificar si dos archivos son iguales
int sonIguales(const char *archivo1, const char *archivo2)
{
    struct stat stat1, stat2;

    if (stat(archivo1, &stat1) != 0 || stat(archivo2, &stat2) != 0)
    {
        return -1;
    }

    if (stat1.st_dev == stat2.st_dev && stat1.st_ino == stat2.st_ino)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Función para escribir un array de strings en un archivo
void escribir_en_archivo(char *nombre_archivo, char **contenido)
{
    FILE *archivo = fopen(nombre_archivo, "w+");

    if (archivo == NULL)
    {
        fprintf(stderr, "reverse: cannot open file '%s'\n", nombre_archivo);
        exit(1);
    }

    // Escribir cada cadena hasta encontrar un puntero nulo
    while (*contenido != NULL)
    {
        if (fputs(*contenido, archivo) == EOF)
        {
            fclose(archivo);
            fprintf(stderr, "Error al escribir en el archivo.\n");
            exit(1);
        }
        contenido++; // Avanzar al siguiente puntero
    }

    fclose(archivo);
}

// Función para imprimir un array de strings
void imprimir_array(char **array)
{
    while (*array != NULL)
    {
        printf("%s", *array);
        array++;
    }
}

// Función principal
int main(int argc, char *argv[])
{
    char **textos = NULL;

    if (argc == 1)
    {
        // Si no se proporcionan argumentos, se obtiene texto desde la consola
        textos = obtener_texto_consola();
        invertir_textos(textos);
        imprimir_array(textos);
        free(textos);
        return 0;
    }
    else if (argc <= 3)
    {
        // Si se proporciona uno o dos argumentos, se carga un archivo y se invierte su contenido
        textos = cargar_archivo(argv[1]);
        invertir_textos(textos);

        if (argc == 3)
        {
            // Si se proporcionan dos argumentos, se escribe el contenido invertido en un nuevo archivo
            if (sonIguales(argv[1], argv[2]) == 1)
            {
                fprintf(stderr, "reverse: input and output file must differ\n");
                return 1;
            }

            escribir_en_archivo(argv[2], textos);
            return 0;
        }
        imprimir_array(textos);
        free(textos);
        return 0;
    }
    else
    {
        // Si se proporcionan más de tres argumentos, se muestra un mensaje de uso incorrecto
        fprintf(stderr, "usage: reverse <input> <output>\n");
        return 1;
    }
}
