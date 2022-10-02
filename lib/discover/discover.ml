module C = Configurator.V1

let from_pkg_config c =
  let fallback = ([], [ "-lX11" ]) in
  match C.Pkg_config.get c with
  | None -> fallback
  | Some pc -> (
      match C.Pkg_config.query pc ~package:"x11" with
      | None -> fallback
      | Some { cflags; libs } -> (cflags, libs))

let () =
  C.main ~name:"discover" (fun c ->
      let cflags, libs = from_pkg_config c
      in
      C.Flags.write_sexp "cflags" cflags;
      C.Flags.write_sexp "libs" libs)
