#use "syst_classes.ml"

let win = new dwindow ~title:"My window" ~size:(500,500) ~pos:(10,10) ();;

(* récupère le gestionnaire des éléments de la fenêtre *)
let cont = win#getContainer ();;

(* exemple de création d'un élément *)
let button = new dbutton ();;
button#setLabel "clic";;
(* paramétrer la taille du bouton n'est pas toujours
obligatoire selon le layout *)
button#setSize (20,20);;

(* ajoute le bouton en position (10,10) *)
cont#addElem button (10, 10);;

let second_cont = new container ();;

second_cont#setSize (50,50);;

cont#addElem second_cont (100,100);;

let second_button = new dbutton ();;
second_button#setLabel "clac";;
button#setSize (40,40);;

second_cont#addElem second_button (0,0);;

(* dessine la fenêtre *)
win#draw ();;