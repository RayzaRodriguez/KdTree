#include <iostream>
#include <vector>
#include <memory>
#include <stack>

using namespace std;


struct Punto {
    float* data;
    int kd{};

    Punto() {
        this->data = nullptr;
    }
    explicit Punto(int kd) {
        this->data = new float[kd];
    }
    Punto(int kd, float* data) {
        this->kd = kd;
        this->data = new float[kd];
        for (int i = 0; i < kd; ++i) {
            this->data[i] = data[i];
        }
    }

    bool operator==(const Punto& rhs) const;
    bool operator!=(const Punto& rhs) const;
    Punto& operator=(const Punto& rhs) const;

    friend ostream& operator<<(ostream& os, const Punto& punto);

    float* getData() const;

    void setData(float* data);

    int getKd() const;

    void setKd(int kd);
};
bool Punto::operator==(const Punto& rhs) const {
    bool flag = false;
    for (int i = 0; this->kd == rhs.kd && i < rhs.kd; ++i) {
        flag = (this->data[i] == rhs.data[i]);
    }
    return flag;
}

bool Punto::operator!=(const Punto& rhs) const {
    return !(rhs == *this);
}

Punto& Punto::operator=(const Punto& rhs) const {
    for (int i = 0; this->kd == rhs.kd && i < rhs.kd; ++i) {
        this->data[i] = rhs.data[i];
    }
    return const_cast<Punto&>(*this);
}

ostream& operator<<(ostream& os, const Punto& punto) {
    os << "memoria: " << punto.data << endl;
    for (int i = 0; punto.data && i < punto.kd; ++i) {
        os << "dataDimension_" << i << ": " << punto.data[i] << endl;
    }
    os << endl;
    return os;
}

float* Punto::getData() const {
    return data;
}

void Punto::setData(float* data) {
    Punto::data = data;
}

int Punto::getKd() const {
    return kd;
}

void Punto::setKd(int kd) {
    Punto::kd = kd;
}


struct Nodo {
    Punto punto;

    Nodo* sgte[2]{};
    Nodo() {
        this->sgte[0] = nullptr;
        this->sgte[1] = nullptr;
    }
    explicit Nodo(Punto punto) {
        this->punto.setKd(punto.getKd());
        this->punto.setData(new float(punto.getKd()));
        this->punto = punto;
        this->sgte[0] = nullptr;
        this->sgte[1] = nullptr;
    }
    Nodo(Punto punto, Nodo* izq, Nodo* der) {
        this->punto = punto;
        this->sgte[0] = izq;
        this->sgte[1] = der;
    }

    bool operator==(const Nodo& rhs) const;
    bool operator!=(const Nodo& rhs) const;

    const Punto& getPunto() const;

    void setPunto(const Punto& punto) const;

    Nodo* const* getSgte() const;
};

bool Nodo::operator==(const Nodo& rhs) const {
    return punto == rhs.punto &&
        sgte == rhs.sgte;
}

bool Nodo::operator!=(const Nodo& rhs) const {
    return !(rhs == *this);
}

const Punto& Nodo::getPunto() const {
    return punto;
}

void Nodo::setPunto(const Punto& punto) const {
    Nodo::punto = punto;
}

Nodo* const* Nodo::getSgte() const {
    return sgte;
}

struct KDtree {
    Nodo* raiz;
    int kd;
    

    explicit KDtree(int kd) {
        this->kd = kd;
        this->raiz = nullptr;
    }
 
//
    bool buscar(Punto p, Nodo**& nodoActual) {
        int altura = 0;

        for (
            nodoActual = &raiz;
            *nodoActual && (*nodoActual)->getPunto() != p;
            nodoActual = &(
                (*nodoActual)->sgte[
                    (*nodoActual)->punto.data[altura % kd] <= p.getData()[altura % kd]]
                )
            ) {
            altura++;
        }

        return (*nodoActual && (*nodoActual)->getPunto() == p);
    }

    bool insertar(Punto p) {
        Nodo** nodoActual;
        if (buscar(p, nodoActual)) {
            return false;
        }
        *nodoActual = new Nodo(p);
        return true;
    }

    static Nodo** rep(Nodo** pNodo) {
        bool dir = rand() % 2;
        for (
            pNodo = &((*pNodo)->sgte[dir]);
            (*pNodo)->sgte[!dir];
            pNodo = &((*pNodo)->sgte[!dir])
            );
        return pNodo;
    }


    bool remover(Punto x)
    {
        Nodo** p;
        if (!buscar(x, p)) return false;
        if ((*p)->sgte[0] && (*p)->sgte[1])
        {
            Nodo** q = rep(p);
            (*p)->getPunto() = (*q)->getPunto();
            p = q;
        }
        Nodo* t = *p;
        *p = (*p)->sgte[!(*p)->sgte[0]];
        delete t;
        return false;
    }

    void inOrderRecursivo(Nodo* n)
    {
        if (!n) return;
        inOrderRecursivo(n->sgte[0]);
        cout << n->getPunto();
        inOrderRecursivo(n->sgte[1]);
    }

    void inOrderIterativo(Nodo* n) const
    {
        stack<Nodo*> pila;
        Nodo* n_actual = raiz;
        while (!pila.empty() || n_actual != nullptr) {
            if (n_actual != nullptr) {
                pila.push(n_actual);
                n_actual = n_actual->sgte[0];
            }
            else {
                n_actual = pila.top();
                pila.pop();
                cout << n_actual->getPunto() << endl;
                n_actual = n_actual->sgte[1];
            }
        }
    }

    void print() const
    {
        if (!raiz) {
            cout << "vacio" << endl << endl;
        }
        else {
            cout << "datos: " << endl;
            inOrderIterativo(raiz);
            cout << endl;
        }

    }
};

using namespace std;

int main() {
    int kd_n = 5;
    float f0[5] = { 10, 20, 30, 45, 2 };
    float f1[5] = { 3, 50 , 33, 63, 11 };
    float f2[5] = { 13, 25, 5, 110, 41 };
    float f3[5] = { 18, 51, 35, 85, 48 };
    float f4[5] = { 88, 77, 2, 23, 4 };

    Punto punto0(kd_n, f0);
    Punto punto1(kd_n, f1);
    Punto punto2(kd_n, f2);
    Punto punto3(kd_n, f3);
    Punto punto4(kd_n, f4);


    KDtree arbolKD(kd_n);
    arbolKD.print();

    arbolKD.insertar(punto0);
    arbolKD.print();
  

    arbolKD.insertar(punto1);
    arbolKD.print();

    arbolKD.insertar(punto2);
    arbolKD.print();

    arbolKD.insertar(punto3);
    arbolKD.print();

    arbolKD.insertar(punto4);
    arbolKD.print();

    /*
    arbolKD.remover(punto0);
    arbolKD.print();

    arbolKD.remover(punto1);
    arbolKD.print();

    arbolKD.remover(punto2);
    arbolKD.print();

    arbolKD.remover(punto3);
    arbolKD.print();

    arbolKD.remover(punto4);
    arbolKD.print();
    */

    return 0;
}
