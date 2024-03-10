class Node {
private:
  char value[4];
  Node *previous;

public:
  Node();
  Node(char input[], Node *prev);
  void SetPrevious(Node *prev);
  void SetValue(char input[]);
  char *GetValue();
  Node *GetPrev();
  ~Node();
};
