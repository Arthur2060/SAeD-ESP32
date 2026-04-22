class SAeDStateMachine {
    private:
        int current_state = 0;
        
    public:
        const int MAPEAMENTO = 0;
        const int DEMARCACAO = 1;
        const int COLETA = 2;
        const int ESTOCAGEM = 3;
        const int DESPACHE = 4;
};