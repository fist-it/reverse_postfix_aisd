class CharNode {
private:
  char value[4];
  CharNode *previous;

public:
  CharNode();
  CharNode(char input[], CharNode *prev);
  void SetPrevious(CharNode *prev);
  void SetValue(char input[]);
  char *GetValue();
  CharNode *GetPrev();
  ~CharNode();
};

class IntNode {
  int value;
  IntNode *previous;

public:
  IntNode();
  IntNode(int input, IntNode *prev);
  void SetPrevious(IntNode *prev);
  void SetValue(int input);
  int GetValue();
  IntNode *GetPrev();
  ~IntNode();
};
