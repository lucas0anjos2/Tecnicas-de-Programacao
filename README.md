# Tecnicas-de-Programacao
Trabalho Final desenvolvido na disciplina de Técnicas de Programação.
 ## 1 Descrição
O objetivo deste trabalho é implementar um pequeno programa em C++, usando um projeto no Code::Blocks. O trabalho deve ser desenvolvido de acordo com a Seção 2. O trabalho vale 15 (quinze) ponrtos da média final. Todos os membros do grupo devem trabalhar colaborativamente, discutindo amplamente todas as decisões tomadas na implementação. Trabalhos que não compilam não possuem condições de avaliação e valem automaticamente 0 (zero). Também serão anulados trabalhos cuja cópia da internet for comprovada em qualquer momento, bem como cópia de trabalhos de outros grupos (sendo anuladas todas as cópias). A compilação e os testes serão efetuados em Linux, portanto, é altamente recomendável que o trabalho seja testado, pelo menos uma vez, nessa plataforma. Além dos testes de caixa preta (resolução do problema), serão avaliados no teste de caixa clara:
- clareza e corretude do código;
- conformidade às especificações;
- comentários e endentação correta.

## 2 Especificação
A meta da implementação é a construção do programa seguindo os seguintes critérios:
- O programa deve ser corretamente modularizado (dividido em classes, métodos, etc.);
- O projeto deve ser criado da seguinte forma: nome1-nome2-nome4, onde nome1, nome2 e nome3 devem ser substituídos pelo nome dos membros do grupo, por exemplo: **chicoanosio-josoares-carlosalberto**;
- No cabeçalho do arquivo, deve constar um comentário com a identificação completa dos membros do grupo, incluindo número de matrícula;
- Ao término do trabalho, cada grupo deverá:
  - Selecionar a opção Clean do menu Build, no Code::Blocks;
  - Compactar a pasta do projeto com todo o seu conteúdo, usando o mesmo nome da pasta como nome do arquivo (ex.: nome1-nome2-nome3.zip)   
Deve ser implementado um programa que armazene um conjunto de objetos TAD (tipo abstrato de dados) **intserial** em uma **árvore B**, com o armazenamento de cada página da árvore em um registro de **arquivo tipado**, conforme visto na disciplina. A árvore deve ter um grau mínimo t = 3, especificado na forma de uma constante no cabeçalho da classe. 

A implementação deve manter o menor número possível de páginas em memória. A classe Cabecalho poderá ser alterada, para indicar a raiz da árvore, se for necessário. Cada página da árvore será armazenada em um único registro do arquivo. 

O programa deve apresentar um menu que permita ao usuário:
- inserir um valor na árvore;
- remover um valor na árvore;
- imprimir a árvore completa;
- buscar um valor em uma árvore;
- o menu deve ser reapresentado após cada ação, exceto quando o usuário solicitar o término da execução. 

Durante a Inserção e a remoção, deve ocorrer uma única passada na árvore, da raiz até a página em que a operação for realizada. No caso da remoção, a busca pelo maior predecessor ou pelo menor sucessor deve ser feita com uma única passada, já removendo e ajustando a árvore para que a remoção seja concluída. 

## Prazo de Entrega
Os trabalhos devem ser entregues, impreterivelmente, até a data estipulada abaixo, através do upload dos arquivos através desta tarefa. Serão aceitos trabalhos até às 23:55h dessa data. A partir das 23:56h, a nota do grupo é automaticamente zero e o envio de arquivos será suspenso.
Sob nenhuma hipótese serão aceitos trabalhos enviados por e-mail, mídia  removível ou impressos.
Serão avaliados apenas os arquivos enviados através do Moodle, dentro do prazo estipulado.
