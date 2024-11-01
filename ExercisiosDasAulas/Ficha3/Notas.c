/*

Redirecionamento de Entrada (Scanf)
    No programa pai: 
        int canal[2]
        pipe(canal)
        fork()

    No programa Filho: 
        1 Passo -> Close (0) -> Fechar o stdin 
        2 Passo -> Dup(canal(0)) -> Mudar o canal 0 de sitio, para onde nos queremos 
        3 Passo -> close(canal(0))
        4 Passso -> close(canal(1))

    


*/