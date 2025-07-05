#include <iostream>
#include <algorithm>

using namespace std;

class Publicacion {
protected:
    string titulo;
    int ano;
public:
    Publicacion(string t, int a) : titulo(move(t)), ano(a) {}
    virtual void mostrar() const = 0;
    virtual string tipo() const = 0;
    virtual ~Publicacion() {}
    string getTitulo() const { return titulo; }
    int getAno() const { return ano; }
};

class Libro : public Publicacion {
    int paginas;
public:
    Libro(string t, int a, int p) : Publicacion(move(t), a), paginas(p) {}
    void mostrar() const override { cout << "[Libro] \"" << titulo << "\" - Ano: " << ano << ", Paginas: " << paginas << '\n'; }
    string tipo() const override { return "Libro"; }
};

class Revista : public Publicacion {
    int edicion;
public:
    Revista(string t, int a, int e) : Publicacion(move(t), a), edicion(e) {}
    void mostrar() const override { cout << "[Revista] \"" << titulo << "\" - Ano: " << ano << ", Edicion: " << edicion << '\n'; }
    string tipo() const override { return "Revista"; }
};

class Periodico : public Publicacion {
    string fecha;
public:
    Periodico(string t, int a, string f) : Publicacion(move(t), a), fecha(move(f)) {}
    void mostrar() const override { cout << "[Periodico] \"" << titulo << "\" - Ano: " << ano << ", Fecha: " << fecha << '\n'; }
    string tipo() const override { return "Periodico"; }
};

void limpiarBuffer() { while (cin.get() != '\n'); }
string leerTexto(const string& mensaje) { string input; do { cout << mensaje; getline(cin, input); } while (input.empty()); return input; }
int leerEntero(const string& mensaje, int min = 1) { int val; do { cout << mensaje; cin >> val; limpiarBuffer(); } while (val < min); return val; }
bool validarAno(int a) { return a >= 1500 && a <= 2025; }

void agregarPublicacion(Publicacion**& catalogo, int& nroPublicaciones) {
    cout << "\nTipo (1-Libro, 2-Revista, 3-Periodico, 4-Regresar al menu principal): ";
    int op; cin >> op; limpiarBuffer();
    
    if (op == 4) {
        cout << "Regresando al menú principal...\n";
        return;
    }

    string t = leerTexto("Titulo: ");
    int a = leerEntero("Ano (>=1500): ", 1500);
    if (!validarAno(a)) { cout << "Ano fuera de rango.\n"; return; }
    
    Publicacion* p = nullptr;
    if (op == 1) p = new Libro(t, a, leerEntero("Paginas: "));
    else if (op == 2) p = new Revista(t, a, leerEntero("Edicion: "));
    else if (op == 3) p = new Periodico(t, a, leerTexto("Fecha: "));
    else { cout << "Opcion invalida.\n"; return; }

    Publicacion** nuevo = new Publicacion*[nroPublicaciones + 1];
    for (int i = 0; i < nroPublicaciones; ++i) nuevo[i] = catalogo[i];
    nuevo[nroPublicaciones++] = p;
    delete[] catalogo;
    catalogo = nuevo;
}

void mostrarCatalogo(Publicacion** catalogo, int nroPublicaciones) {
    if (nroPublicaciones == 0) cout << "No hay publicaciones.\n";
    else for (int i = 0; i < nroPublicaciones; ++i) {
        cout << i + 1 << ". "; catalogo[i]->mostrar();
    }
}

void buscarTitulo(Publicacion** catalogo, int nroPublicaciones) {
    string clave = leerTexto("Buscar titulo: ");
    transform(clave.begin(), clave.end(), clave.begin(), ::tolower);
    bool found = false;
    for (int i = 0; i < nroPublicaciones; ++i) {
        string t = catalogo[i]->getTitulo();
        transform(t.begin(), t.end(), t.begin(), ::tolower);
        if (t.find(clave) != string::npos) { catalogo[i]->mostrar(); found = true; }
    }
    if (!found) cout << "No se encontraron coincidencias.\n";
}

void eliminarPublicacion(Publicacion**& catalogo, int& nroPublicaciones) {
    mostrarCatalogo(catalogo, nroPublicaciones);
    if (nroPublicaciones == 0) return;
    int idx = leerEntero("Seleccione numero a eliminar: ") - 1;
    if (idx < 0 || idx >= nroPublicaciones) { cout << "Indice invalido.\n"; return; }
    delete catalogo[idx];
    for (int i = idx; i < nroPublicaciones - 1; ++i) catalogo[i] = catalogo[i + 1];
    Publicacion** nuevo = new Publicacion*[--nroPublicaciones];
    for (int i = 0; i < nroPublicaciones; ++i) nuevo[i] = catalogo[i];
    delete[] catalogo;
    catalogo = nuevo;
    cout << "Publicacion eliminada.\n";
}

void mostrarEstadisticas(Publicacion** catalogo, int nroPublicaciones) {
    int libros=0, revistas=0, periodicos=0, minAno=9999, maxAno=0;
    for (int i=0; i<nroPublicaciones; ++i) {
        string t = catalogo[i]->tipo();
        if (t=="Libro") libros++;
        else if (t=="Revista") revistas++;
        else if (t=="Periodico") periodicos++;
        int a = catalogo[i]->getAno();
        if (a < minAno) minAno = a;
        if (a > maxAno) maxAno = a;
    }
    cout << "\nEstadisticas:\nTotal: " << nroPublicaciones
         << "\nLibros: " << libros << ", Revistas: " << revistas << ", Periodicos: " << periodicos << '\n';
    if (nroPublicaciones) cout << "Ano mas antiguo: " << minAno << ", mas reciente: " << maxAno << '\n';
}

void liberarMemoria(Publicacion**& catalogo, int& nroPublicaciones) {
    for (int i=0; i<nroPublicaciones; ++i) delete catalogo[i];
    delete[] catalogo;
    catalogo = nullptr; nroPublicaciones = 0;
}

int main() {
    Publicacion** catalogo = nullptr;
    int nroPublicaciones = 0, opcion;
    do {
        cout << "MENU PRINCIPAL\n1.Agregar\n2.Mostrar\n3.Buscar\n4.Eliminar\n5.Estadisticas\n6.Salir\n> ";
        cin >> opcion; limpiarBuffer();
        switch (opcion) {
            case 1: agregarPublicacion(catalogo, nroPublicaciones); break;
            case 2: mostrarCatalogo(catalogo, nroPublicaciones); break;
            case 3: buscarTitulo(catalogo, nroPublicaciones); break;
            case 4: eliminarPublicacion(catalogo, nroPublicaciones); break;
            case 5: mostrarEstadisticas(catalogo, nroPublicaciones); break;
            case 6: liberarMemoria(catalogo, nroPublicaciones); cout << "Saliendo...\n"; break;
            default: cout << "Opcion invalida.\n"; break;
        }
    } while (opcion != 6);
    return 0;
}
