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
    cout << "\nTipo (1-Libro, 2-Revista, 3-Periodico): ";
    int op; cin >> op; limpiarBuffer();
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



