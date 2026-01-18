Alt_Exes folder:

U tom folderu su alternativni exe-ovi ako slučajno windows detektira originalni exe kao virus (ponekad detektira,a ponekad ne). U slućaju da detektira,koristiti jedan od alternativnih exe (nije ništa drugacije osim što sam postepeno brisao dio koda koji detektira kao virus). Virus je false-positive,a detektira ga jer koristim windows.h za aktivaciju virtualnog terminala,širenje prozora preko ekrana i promjena fonta. Kod alternativnih exe-ova, ako igra nije ispravno prikazana, treba uključiti virtual terminal processing, a najlakše se uključi tako što se otvori command prompt i upiše: "reg add HKCU\Console /v VirtualTerminalLevel /t REG_DWORD /d 1 /f".

Općenito o igri:

Na početku te igra pita koje opcije želis uključiti. Opcije odabireš tako da napišeš "Y" za da, a "N" za ne. npr YYN znači: Fast rounds - Da, Player 1 as bot - Da, See enemy ships - Ne.
Nakon što odabereš opcije trebaš postaviti svojih 5 brodova. Prvo se postavlja najveći brod pa se ide prema najmanjem. Napišeš poziciju i onda odabereš jednu od ponuđenih opcija (Down - dolje, Up - gore, Left - lijevo, Right - desno). Ako se predomisliš možes napisati "Undo" i ponovno možes izabrati poziciju. Također možes napisati "auto" umisto da napišeš poziciju, a to će automatski postaviti ostatak brodova.
Nakon toga napadaš neprijatelja tako da upišeš koordinatu koju želis napasati. Nakon svog napada, drugi igrać napada tebe i onda ponovno vi njega. Igra se sve dok jedan od igraća potoni sve neprijateljske brodove.