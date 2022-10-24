external createWindow : string -> nativeint = "createWindow_cpp"
external sendMessage : nativeint -> int -> unit = "sendMessage_cpp"
external waitForClose : nativeint -> unit = "waitForClose_cpp"
