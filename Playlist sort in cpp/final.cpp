#include <iostream>
#include <cstdlib>
#include <windows.h>

using namespace std;

struct Nodo
{
    string titulo;
    string artista;
    string genero;
    int anio;
    int duracionSeg;
    int duracionMin;
    int duracionTOTAL;
    Nodo *sig;
};

void color(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void resetColor()
{
    color(7);
}

void buscarPorTitulo(Nodo *F);
void cancionesPrueba(Nodo *&, Nodo *&);
void dividirLista(Nodo *, Nodo **, Nodo **);
void eliminar(Nodo *&);
void ingresar(Nodo *&, Nodo *&);
void mostrar(Nodo *);
Nodo *mergeArtista(Nodo *a, Nodo *b, bool ascendente);
Nodo *mergeDuracion(Nodo *a, Nodo *b, bool ascendente);
Nodo *mergeSortArtista(Nodo *cabeza, bool ascendente);
Nodo *mergeSortDuracion(Nodo *cabeza, bool ascendente);
Nodo *mergeSortTitulo(Nodo *, bool);
Nodo *mergeTitulo(Nodo *, Nodo *, bool);
Nodo *obtenerUltimo(Nodo *nodo);
void ordenarBurbujaAnio(Nodo *&, bool);
Nodo *particionarGenero(Nodo *cabeza, Nodo **nuevoInicio, Nodo **nuevoFin, bool ascendente);
Nodo *quickSortGenero(Nodo *cabeza, bool ascendente);

int main()
{
    Nodo *P = NULL;
    Nodo *F = NULL;
    char opcion;

    do
    {
        system("cls");
        color(11);
        cout << "\tPLAYLIST" << endl;
        cout << "------------------------------" << endl;
        resetColor();
        cout << "1. Agregar cancion a la fila" << endl;
        cout << "2. Reproducir cancion" << endl;
        cout << "3. Mostrar fila de reproduccion" << endl;
        cout << "4. Ordenar playlist" << endl;
        cout << "5. Agregar canciones de prueba" << endl;
        cout << "6. Buscar cancion por titulo" << endl;
        cout << "7. Salir" << endl;
        cout << "------------------------------" << endl;
        color(10);
        cout << "Seleccione una opcion: ";
        resetColor();
        cin >> opcion;

        switch (opcion)
        {
        case '1':
            ingresar(P, F);
            break;
        case '2':
            eliminar(F);
            break;
        case '3':
            mostrar(F);
            break;
        case '4':
        {
            char subop;
            system("cls");
            color(10);
            cout << "Ordenar por:" << endl;
            resetColor();
            cout << "1. Titulo" << endl;
            cout << "2. Artista" << endl;
            cout << "3. Anio" << endl;
            cout << "4. Duracion" << endl;
            cout << "5. Genero" << endl;
            cout << "6. Cancelar" << endl;
            color(10);
            cout << "Seleccion: ";
            resetColor();
            cin >> subop;

            if (subop >= '1' && subop <= '5')
            {
                char orden;
                cout << "Desea ordenar de forma ascendente (A) o descendente (D)? ";
                cin >> orden;
                orden = (orden == 'a' || orden == 'A') ? 'A' : 'D';

                switch (subop)
                {
                case '1':
                    F = mergeSortTitulo(F, orden == 'A');
                    break;
                case '2':
                    F = mergeSortArtista(F, orden == 'A');
                    break;
                case '3':
                    ordenarBurbujaAnio(F, orden == 'A');
                    break;
                case '4':
                    F = mergeSortDuracion(F, orden == 'A');
                    P = F;
                    while (P && P->sig)
                        P = P->sig;
                    break;

                case '5':
                    F = quickSortGenero(F, orden == 'A');
                    break;
                }

                P = F;
                while (P && P->sig)
                    P = P->sig;

                if (subop != '4')
                {
                    color(10);
                    string campos[] = {"titulo", "artista", "anio", "duracion", "genero"};
                    cout << "\nPlaylist ordenada por " << campos[subop - '1']
                         << " en orden " << (orden == 'A' ? "ascendente.\n" : "descendente.\n");
                    resetColor();
                }

                system("pause");
            }
            else if (subop != '6')
            {
                color(12);
                cout << "\nOpcion invalida.\n";
                resetColor();
                system("pause");
            }
            break;
        }
        case '5':
            cancionesPrueba(P, F);
            break;
        case '6':
            buscarPorTitulo(F);
            break;
        case '7':
            color(10);
            cout << "\nSaliendo del programa...\n";
            resetColor();
            break;

        default:
            color(12);
            cout << "\nError: Opcion invalida.\n";
            resetColor();
            system("pause");
        }

    } while (opcion != '7');

    return 0;
}

void ingresar(Nodo *&P, Nodo *&F)
{
    int seg = 0;
    Nodo *nuevo = new Nodo;

    cout << "\nIngrese titulo: ";
    cin.ignore();
    getline(cin, nuevo->titulo);
    cout << "Ingrese artista: ";
    getline(cin, nuevo->artista);
    cout << "Ingrese genero: ";
    getline(cin, nuevo->genero);
    cout << "Ingrese fecha de lanzamiento (anio): ";
    cin >> nuevo->anio;
    cout << "Ingrese duracion (segundos): ";
    cin >> seg;

    if (seg <= 0)
    {
        color(14);
        cout << "\nAdvertencia: La duracion debe ser mayor a 0.\n";
        resetColor();
        system("pause");
        delete nuevo;
        return;
    }

    nuevo->duracionMin = seg / 60;
    nuevo->duracionSeg = seg % 60;
    nuevo->duracionTOTAL = seg;
    nuevo->sig = NULL;

    if (F == NULL)
    {
        P = nuevo;
        F = nuevo;
    }
    else
    {
        P->sig = nuevo;
        P = nuevo;
    }

    color(10);
    cout << "\nCancion agregada exitosamente.\n";
    resetColor();
    system("pause");
}

void eliminar(Nodo *&F)
{
    if (F == NULL)
    {
        color(14);
        cout << "\nAdvertencia: Fila vacia, no hay canciones aun.\n\n";
        resetColor();
    }
    else
    {
        color(10);
        cout << "\nReproduciendo: " << F->titulo << " - " << F->artista << endl;
        resetColor();
        Nodo *temp = F;
        F = F->sig;
        delete temp;
    }
    system("pause");
}

void mostrar(Nodo *F)
{
    if (F == NULL)
    {
        color(14);
        cout << "\nAdvertencia: Fila vacia, no hay canciones aun.\n\n";
        resetColor();
    }
    else
    {
        color(11);
        cout << "\n-----------------------------" << endl;
        cout << "|     PLAYLIST ACTUAL      |" << endl;
        cout << "-----------------------------" << endl;
        resetColor();
        while (F != NULL)
        {
            color(11);
            cout << "Titulo:   ";
            resetColor();
            cout << F->titulo << endl;
            color(11);
            cout << "Artista:  ";
            resetColor();
            cout << F->artista << endl;
            color(11);
            cout << "Genero:   ";
            resetColor();
            cout << F->genero << endl;
            color(11);
            cout << "Anio:     ";
            resetColor();
            cout << F->anio << endl;
            color(11);
            cout << "Duracion: ";
            resetColor();
            cout << F->duracionMin << ":";
            if (F->duracionSeg < 10)
                cout << "0";
            cout << F->duracionSeg << endl;
            color(11);
            cout << "-----------------------------" << endl;
            resetColor();
            F = F->sig;
        }
    }
    system("pause");
}

Nodo *mergeSortTitulo(Nodo *cabeza, bool ascendente)
{
    if (cabeza == NULL || cabeza->sig == NULL)
        return cabeza;

    Nodo *a, *b;
    dividirLista(cabeza, &a, &b);

    a = mergeSortTitulo(a, ascendente);
    b = mergeSortTitulo(b, ascendente);

    return mergeTitulo(a, b, ascendente);
}

void dividirLista(Nodo *fuente, Nodo **frontRef, Nodo **backRef)
{
    Nodo *rapido;
    Nodo *lento;
    if (fuente == NULL || fuente->sig == NULL)
    {
        *frontRef = fuente;
        *backRef = NULL;
    }
    else
    {
        lento = fuente;
        rapido = fuente->sig;

        while (rapido != NULL)
        {
            rapido = rapido->sig;
            if (rapido != NULL)
            {
                lento = lento->sig;
                rapido = rapido->sig;
            }
        }

        *frontRef = fuente;
        *backRef = lento->sig;
        lento->sig = NULL;
    }
}

Nodo *mergeTitulo(Nodo *a, Nodo *b, bool ascendente)
{
    Nodo *resultado = NULL;

    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    bool condicion = ascendente ? (a->titulo <= b->titulo) : (a->titulo >= b->titulo);

    if (condicion)
    {
        resultado = a;
        resultado->sig = mergeTitulo(a->sig, b, ascendente);
    }
    else
    {
        resultado = b;
        resultado->sig = mergeTitulo(a, b->sig, ascendente);
    }

    return resultado;
}

void cancionesPrueba(Nodo *&P, Nodo *&F)
{
    string titulos[] = {
        "Despacito", "La Bicicleta", "Bailando", "Vivir Mi Vida", "Rayando el Sol",
        "Me Gustas Tu", "Eres", "Andar Conmigo", "Limon y Sal", "Colgando en tus manos",
        "Shape of You", "Blinding Lights", "Stay", "Uptown Funk", "Perfect",
        "Levitating", "Bad Habits", "Shivers", "Senorita", "Love Yourself"};
    string artistas[] = {
        "Luis Fonsi", "Carlos Vives", "Enrique Iglesias", "Marc Anthony", "Mana",
        "Manu Chao", "Cafe Tacvba", "Julieta Venegas", "Julieta Venegas", "Carlos Baute",
        "Ed Sheeran", "The Weeknd", "Justin Bieber", "Bruno Mars", "Ed Sheeran",
        "Dua Lipa", "Ed Sheeran", "Ed Sheeran", "Shawn Mendes", "Justin Bieber"};
    string generos[] = {
        "Reggaeton", "Pop Latino", "Pop", "Salsa", "Rock Latino",
        "Folk", "Rock Alternativo", "Pop", "Pop", "Balada",
        "Pop", "Synthpop", "Pop", "Funk", "Balada",
        "Pop", "Pop", "Pop", "Pop", "Pop"};
    int anios[] = {
        2017, 2016, 2014, 2013, 1999,
        2001, 2003, 2003, 2006, 2008,
        2017, 2020, 2021, 2014, 2017,
        2021, 2021, 2021, 2019, 2015};
    int duraciones[] = {
        230, 250, 240, 220, 210,
        180, 200, 190, 185, 200,
        235, 200, 210, 240, 270,
        220, 250, 260, 230, 215};

    for (int i = 0; i < 20; i++)
    {
        Nodo *nuevo = new Nodo;
        nuevo->titulo = titulos[i];
        nuevo->artista = artistas[i];
        nuevo->genero = generos[i];
        nuevo->anio = anios[i];
        nuevo->duracionTOTAL = duraciones[i];
        nuevo->duracionMin = duraciones[i] / 60;
        nuevo->duracionSeg = duraciones[i] % 60;
        nuevo->sig = NULL;

        if (F == NULL)
        {
            P = nuevo;
            F = nuevo;
        }
        else
        {
            P->sig = nuevo;
            P = nuevo;
        }
    }
    color(10);
    cout << "\n20 canciones de prueba agregadas correctamente.\n";
    resetColor();
    system("pause");
}

void ordenarBurbujaAnio(Nodo *&F, bool ascendente)
{
    if (F == NULL || F->sig == NULL)
        return;

    int n = 0;
    Nodo *cont = F;
    while (cont != NULL)
    {
        n++;
        cont = cont->sig;
    }

    for (int i = 0; i < n - 1; i++)
    {
        Nodo *actual = F;
        Nodo *siguiente = F->sig;

        for (int j = 0; j < n - i - 1; j++)
        {
            bool condicion = ascendente ? (actual->anio > siguiente->anio) : (actual->anio < siguiente->anio);

            if (condicion)
            {
                string t, a, g;
                int an, ds, dm, dt;

                t = actual->titulo;
                a = actual->artista;
                g = actual->genero;
                an = actual->anio;
                ds = actual->duracionSeg;
                dm = actual->duracionMin;
                dt = actual->duracionTOTAL;

                actual->titulo = siguiente->titulo;
                actual->artista = siguiente->artista;
                actual->genero = siguiente->genero;
                actual->anio = siguiente->anio;
                actual->duracionSeg = siguiente->duracionSeg;
                actual->duracionMin = siguiente->duracionMin;
                actual->duracionTOTAL = siguiente->duracionTOTAL;

                siguiente->titulo = t;
                siguiente->artista = a;
                siguiente->genero = g;
                siguiente->anio = an;
                siguiente->duracionSeg = ds;
                siguiente->duracionMin = dm;
                siguiente->duracionTOTAL = dt;
            }

            actual = actual->sig;
            siguiente = siguiente->sig;
        }
    }
}

Nodo *obtenerUltimo(Nodo *nodo)
{
    while (nodo && nodo->sig)
        nodo = nodo->sig;
    return nodo;
}

Nodo *particionarGenero(Nodo *cabeza, Nodo **nuevoInicio, Nodo **nuevoFin, bool ascendente)
{
    Nodo *pivote = obtenerUltimo(cabeza);
    Nodo *anterior = NULL, *actual = cabeza, *cola = pivote;
    Nodo *nuevoCabeza = NULL, *nuevoCola = pivote;

    while (actual != pivote)
    {
        bool condicion = ascendente ? (actual->genero <= pivote->genero) : (actual->genero >= pivote->genero);

        if (condicion)
        {
            if (!nuevoCabeza)
                nuevoCabeza = actual;

            anterior = actual;
            actual = actual->sig;
        }
        else
        {
            Nodo *temp = actual->sig;

            if (anterior)
                anterior->sig = temp;
            else
                cabeza = temp;

            actual->sig = NULL;
            nuevoCola->sig = actual;
            nuevoCola = actual;

            actual = temp;
        }
    }

    if (!nuevoCabeza)
        nuevoCabeza = pivote;

    *nuevoInicio = nuevoCabeza;
    *nuevoFin = nuevoCola;

    return pivote;
}

Nodo *quickSortGeneroRecur(Nodo *cabeza, Nodo *fin, bool ascendente)
{
    if (!cabeza || cabeza == fin)
        return cabeza;

    Nodo *nuevoInicio = NULL, *nuevoFin = NULL;
    Nodo *pivote = particionarGenero(cabeza, &nuevoInicio, &nuevoFin, ascendente);

    if (nuevoInicio != pivote)
    {
        Nodo *temp = nuevoInicio;
        while (temp->sig != pivote)
            temp = temp->sig;
        temp->sig = NULL;

        nuevoInicio = quickSortGeneroRecur(nuevoInicio, temp, ascendente);

        temp = obtenerUltimo(nuevoInicio);
        temp->sig = pivote;
    }

    pivote->sig = quickSortGeneroRecur(pivote->sig, nuevoFin, ascendente);

    return nuevoInicio;
}

Nodo *quickSortGenero(Nodo *cabeza, bool ascendente)
{
    Nodo *fin = obtenerUltimo(cabeza);
    return quickSortGeneroRecur(cabeza, fin, ascendente);
}

Nodo *mergeSortArtista(Nodo *cabeza, bool ascendente)
{
    if (cabeza == NULL || cabeza->sig == NULL)
        return cabeza;

    Nodo *a, *b;
    dividirLista(cabeza, &a, &b);

    a = mergeSortArtista(a, ascendente);
    b = mergeSortArtista(b, ascendente);

    return mergeArtista(a, b, ascendente);
}
Nodo *mergeArtista(Nodo *a, Nodo *b, bool ascendente)
{
    Nodo *resultado = NULL;

    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    bool condicion = ascendente ? (a->artista <= b->artista) : (a->artista >= b->artista);

    if (condicion)
    {
        resultado = a;
        resultado->sig = mergeArtista(a->sig, b, ascendente);
    }
    else
    {
        resultado = b;
        resultado->sig = mergeArtista(a, b->sig, ascendente);
    }

    return resultado;
}
Nodo *mergeDuracion(Nodo *a, Nodo *b, bool ascendente)
{
    if (a == NULL)
        return b;
    if (b == NULL)
        return a;

    Nodo *resultado = NULL;
    bool condicion = ascendente ? (a->duracionTOTAL <= b->duracionTOTAL)
                                : (a->duracionTOTAL >= b->duracionTOTAL);

    if (condicion)
    {
        resultado = a;
        resultado->sig = mergeDuracion(a->sig, b, ascendente);
    }
    else
    {
        resultado = b;
        resultado->sig = mergeDuracion(a, b->sig, ascendente);
    }

    return resultado;
}

Nodo *mergeSortDuracion(Nodo *cabeza, bool ascendente)
{
    if (cabeza == NULL || cabeza->sig == NULL)
        return cabeza;

    Nodo *a;
    Nodo *b;
    dividirLista(cabeza, &a, &b);

    a = mergeSortDuracion(a, ascendente);
    b = mergeSortDuracion(b, ascendente);

    return mergeDuracion(a, b, ascendente);
}
void buscarPorTitulo(Nodo *F)
{
    if (F == NULL)
    {
        color(14);
        cout << "\nAdvertencia: La lista esta vacia.\n\n";
        resetColor();
        system("pause");
        return;
    }
    cin.ignore();
    string tituloBuscado;
    cout << "\nIngrese el titulo a buscar: ";
    getline(cin, tituloBuscado);

    bool encontrado = false;
    while (F != NULL)
    {
        if (F->titulo.find(tituloBuscado) != string::npos)
        {
            if (!encontrado)
            {
                color(11);
                cout << "\nResultados encontrados:\n";
                cout << "-----------------------------" << endl;
                resetColor();
            }

            encontrado = true;
            color(11);
            cout << "Titulo:   ";
            resetColor();
            cout << F->titulo << endl;
            color(11);
            cout << "Artista:  ";
            resetColor();
            cout << F->artista << endl;
            color(11);
            cout << "Genero:   ";
            resetColor();
            cout << F->genero << endl;
            color(11);
            cout << "Anio:     ";
            resetColor();
            cout << F->anio << endl;
            color(11);
            cout << "Duracion: ";
            resetColor();
            cout << F->duracionMin << ":";
            if (F->duracionSeg < 10)
                cout << "0";
            cout << F->duracionSeg << endl;
            color(11);
            cout << "-----------------------------" << endl;
            resetColor();
        }

        F = F->sig;
    }

    if (!encontrado)
    {
        color(12);
        cout << "\nNo se encontraron canciones con ese titulo.\n";
        resetColor();
    }

    system("pause");
}
