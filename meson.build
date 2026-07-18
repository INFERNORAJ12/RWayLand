project(
  'shieldwm',
  'c',
  version: '0.1.0',
  default_options: [
    'c_std=c11',
    'warning_level=2',
  ],
)

add_project_arguments('-DWLR_USE_UNSTABLE', language: 'c')

# Find the required libraries
wayland_server = dependency('wayland-server')
wlroots = dependency('wlroots', version: '>=0.17.0')
xkbcommon = dependency('xkbcommon')

# --- NEW: Generate XDG Shell Protocol Files ---
wayland_scanner = find_program('wayland-scanner')
wayland_protocols = dependency('wayland-protocols')

# Find the path to the XML files on your system
wayland_protocols_dir = wayland_protocols.get_variable(pkgconfig: 'pkgdatadir')
xdg_shell_xml = join_paths(wayland_protocols_dir, 'stable/xdg-shell/xdg-shell.xml')

# Generate the header file
xdg_shell_h = custom_target(
  'xdg-shell-protocol.h',
  command: [wayland_scanner, 'server-header', '@INPUT@', '@OUTPUT@'],
  input: xdg_shell_xml,
  output: 'xdg-shell-protocol.h',
)

# Generate the C source code file
xdg_shell_c = custom_target(
  'xdg-shell-protocol.c',
  command: [wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@'],
  input: xdg_shell_xml,
  output: 'xdg-shell-protocol.c',
)
# ---------------------------------------------

inc = include_directories('include')

# Build our executable, adding the generated files to the source list
executable(
  'shieldwm',
  ['src/main.c', xdg_shell_h, xdg_shell_c],
  include_directories: inc,
  dependencies: [wayland_server, wlroots, xkbcommon],
)