# Mull
O [Mull](https://github.com/mull-project/mull?tab=readme-ov-file) é uma ferramenta de testes de mutação para C/C++ fácil de usar e com considerável suporte da comunidade.

## Tutorial

### Dependências
Para este tutorial, é necessário ter o compilador `clang` instalado. O sistema operacional utilizado para testar o tutorial foi o Ubuntu 24.04.2 LTS.

### Instalação
O Mull não está disponível nos gerenciadores de pacote padrão do Ubuntu. Então, para adicioná-lo, precisamos fazer:

```
$ curl -1sLf 'https://dl.cloudsmith.io/public/mull-project/mull-stable/setup.deb.sh' | sudo -E bash
```

Em seguida, para instalar a versão estável mais recente do Mull:

```
$ sudo apt-get update
$ sudo apt-get install mull-18
```

Após isso, podemos verificar se a instalação foi bem-sucedida, fazendo:

```
$ mull-runner-18 --version
Mull: Practical mutation testing for C and C++
Home: https://github.com/mull-project/mull
Docs: https://mull.readthedocs.io
Support: https://mull.readthedocs.io/en/latest/Support.html
Version: 0.25.0
Commit: 319e8b0
Date: 01 Mar 2025
LLVM: 18.1.3
```

### Hello World
Está tudo pronto para rodar o Mull! Para seguir com o tutorial, clone este repositório e navegue até ele usando:

```
$ git clone git@github.com:RafaelBNN/mull-tutorial.git
$ cd mull-tutorial
```

Agora, para ver o `mull-runner-18` funcionando, precisamos apenas e simplesmente de um arquivo binário. Por agora, vamos compilar o `conditional_calculator.c` e ver o que acontece:

```
$ clang conditional_calculator.c -o conditional_calculator
```

Será criado um arquivo binário `conditional_calculator` na pasta raiz. Agora, podemos fazer:

```
$ mull-runner-18 ./conditional_calculator
[warning] Could not find dynamic library: libc.so.6
[info] Warm up run (threads: 1)
       [################################] 1/1. Finished in 75ms
[info] Filter mutants (threads: 1)
       [################################] 1/1. Finished in 0ms
[info] Baseline run (threads: 1)
       [################################] 1/1. Finished in 69ms
[info] No mutants found. Mutation score: infinitely high
[info] Total execution time: 144ms
```

Perceba que, por enquanto, não foram encontrados mutantes no nosso programa. Isso é porque precisamos dizer explicitamente para o `clang` que queremos inseri-los utilizando o Mull. Podemos fazer isso compilando da seguinte maneira:

```
$ clang -fpass-plugin=/usr/lib/mull-ir-frontend-18 -g -grecord-command-line conditional_calculator.c -o conditional_calculator
```

Agora podemos rodar o Mull novamente e vamos ter o seguinte resultado:

```
$ mull-runner-18 ./conditional_calculator
[warning] Could not find dynamic library: libc.so.6
[info] Warm up run (threads: 1)
       [################################] 1/1. Finished in 68ms
[info] Filter mutants (threads: 1)
       [################################] 1/1. Finished in 0ms
[info] Baseline run (threads: 1)
       [################################] 1/1. Finished in 72ms
[info] Running mutants (threads: 4)
       [################################] 4/4. Finished in 102ms
[info] Survived mutants (4/4):
/home/rafael/pessoal/cin/residencia/automacao-testes/trab-final/mull-tutorial/conditional_calculator.c:5:14: warning: Survived: Replaced >= with > [cxx_ge_to_gt]
    if (num1 >= num2)
             ^
/home/rafael/pessoal/cin/residencia/automacao-testes/trab-final/mull-tutorial/conditional_calculator.c:5:14: warning: Survived: Replaced >= with < [cxx_ge_to_lt]
    if (num1 >= num2)
             ^
/home/rafael/pessoal/cin/residencia/automacao-testes/trab-final/mull-tutorial/conditional_calculator.c:6:21: warning: Survived: Replaced - with + [cxx_sub_to_add]
        return num1 - num2;
                    ^
/home/rafael/pessoal/cin/residencia/automacao-testes/trab-final/mull-tutorial/conditional_calculator.c:8:21: warning: Survived: Replaced + with - [cxx_add_to_sub]
        return num1 + num2;
                    ^
[info] Mutation score: 0%
[info] Total execution time: 242ms
[info] Surviving mutants: 4
```

Foram criados 4 mutantes! Porém, como temos apenas a função `compute()` e nenhum teste, todos eles sobreviveram. E, já que estamos usando testes de mutação, nosso objetivo é justamente testar a qualidade da nossa **suíte de testes**.

Dessa forma, o arquivo `conditional_calculator_with_tests.c` contém uma cópia do método `compute()` e alguns testes na própria `main()`. Eles serão suficientes para um exemplo simples.

Assim, compilando o arquivo `conditional_calculator_with_tests.c` e inserindo mutantes:

```
$ clang -fpass-plugin=/usr/lib/mull-ir-frontend-18 -g -grecord-command-line conditional_calculator_with_tests.c -o conditional_calculator_with_tests
```

Rodando com o `mull-runner-18`:

```
$ mull-runner-18 ./conditional_calculator_with_tests
[warning] Could not find dynamic library: libc.so.6
[info] Warm up run (threads: 1)
       [################################] 1/1. Finished in 69ms
[info] Filter mutants (threads: 1)
       [################################] 1/1. Finished in 0ms
[info] Baseline run (threads: 1)
       [################################] 1/1. Finished in 70ms
[info] Running mutants (threads: 6)
       [################################] 6/6. Finished in 131ms
[info] Survived mutants (2/6):
/home/rafael/pessoal/cin/residencia/automacao-testes/trab-final/mull-tutorial/conditional_calculator_with_tests.c:5:14: warning: Survived: Replaced >= with > [cxx_ge_to_gt]
    if (num1 >= num2)
             ^
/home/rafael/pessoal/cin/residencia/automacao-testes/trab-final/mull-tutorial/conditional_calculator_with_tests.c:6:21: warning: Survived: Replaced - with + [cxx_sub_to_add]
        return num1 - num2;
                    ^
[info] Mutation score: 66%
[info] Total execution time: 271ms
[info] Surviving mutants: 2
```

Percebemos que 2 dos mutantes sobreviveram, e o Mull nos diz quais foram. Dessa forma, podemos identificar onde ocorreram os erros e melhorar nossa suíte de testes. O arquivo `conditional_calculator_with_tests_improved.c` adiciona dois novos casos de teste.

E aí:

```
$ clang -fpass-plugin=/usr/lib/mull-ir-frontend-18 -g -grecord-command-line conditional_calculator_with_tests_improved.c -o conditional_calculator_with_tests_improved
[warning] Mull cannot find config (mull.yml). Using some defaults.

$ mull-runner-18 ./conditional_calculator_with_tests_improved
[warning] Could not find dynamic library: libc.so.6
[info] Warm up run (threads: 1)
       [################################] 1/1. Finished in 69ms
[info] Filter mutants (threads: 1)
       [################################] 1/1. Finished in 0ms
[info] Baseline run (threads: 1)
       [################################] 1/1. Finished in 76ms
[info] Running mutants (threads: 8)
       [################################] 8/8. Finished in 141ms
[info] All mutations have been killed
[info] Mutation score: 100%
[info] Total execution time: 286ms
```

Vimos que conseguimos matar todos os mutantes.

## Conclusão

Dessa forma, podemos ver, com um exemplo simples, como podemos realizar testes de mutação usando o Mull. O Mull ainda permite a configuração dos mutantes criados (por exemplo, poderíamos incluir um mutante para alterar o `'+'` para `'*'`, `'-'` para `'/'`, trocar variáveis por constantes, etc. 

Isso mostra que o Mull é uma ferramenta poderosa para avaliar nossas suítes de testes, identificando pontos fracos e incentivando melhorias. Com essa ferramenta, garantimos não apenas a cobertura do código, mas também que os nossos testes correspondem perfeitamente ao nosso programa.
