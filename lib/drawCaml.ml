external createWindowEx : string -> int -> int -> int -> int -> nativeint = "createWindow_cpp"
external sendMessage : nativeint -> int -> unit = "sendMessage_cpp"
external waitForClose : nativeint -> unit = "waitForClose_cpp"

external setBgColorEx : nativeint -> int -> int -> int -> unit = "setBgColor_cpp"
external createContainerEx : int -> nativeint = "createContainer_cpp"