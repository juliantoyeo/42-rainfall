#include <unistd.h>
#include <cstring>

class N {
  private:
    char annotation[100];
    int nb;

  public:
    N(int nb) {
      this->nb = nb;
    }

    int operator+(N &other) {
      return (this->nb + other.nb);
    }

    int operator-(N &other) {
      return (other.nb - this->nb);
    }

    void *setAnnotation(char* str) {
      return (memcpy(this->annotation, str, strlen(str)));
    }
}

int main(int argc, char** argv) {
  if (argc < 1) {
    _exit(1);
  }

  N *l_var1 = new N(5);
  N *l_var2 = new N(6);

  l_var1->setAnnotation(argv[1]);
  return(**l_var2(l_var2, l_var1)); // this im not sure
}