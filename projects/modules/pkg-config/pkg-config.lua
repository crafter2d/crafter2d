-- pkg-config library from https://github.com/tarruda/premake-pkgconfig

local p = premake

p.modules.pkgconfig = {}
p.modules.pkgconfig._VERSION = p._VERSION

local pkgconfig = p.modules.pkgconfig

local pathsplit
if os.target() == 'windows' then
  pathsplit = '[^;]+'
else
  pathsplit = '[^:]+'
end

pkgconfig.path = {}
pkgconfig.prefix = ''

do
  local tab = {}
  local pkg_config_path = os.getenv('PKG_CONFIG_PATH') 
  if pkg_config_path then
    for str in pkg_config_path:gmatch(pathsplit) do
      tab[str] = true
    end
  end
  for k, _ in pairs(tab) do
    table.insert(pkgconfig.path, k)
  end
end

pkgconfig.path = table.join(pkgconfig.path, { 
  -- debian/ubuntu multilib
  '/usr/lib/x86_64-linux-gnu/pkgconfig',
  -- paths listed in `man pkg-config`
  '/usr/local/share/pkgconfig',
  '/usr/local/lib/pkgconfig',
  '/usr/share/pkgconfig',
  '/usr/lib/pkgconfig'
})

local function interp(s, tab)
  -- source: http://lua-users.org/wiki/StringInterpolation
  return (s:gsub('($%b{})', function(w) return tab[w:sub(3, -2):lower()] or w end))
end

local function strip(str)
  return str:gsub("^%s*(.-)%s*$", "%1")
end

function pkgconfig.setPrefix(prefix)
  pkgconfig.prefix = prefix;
end

function pkgconfig.addPath(name)
  pkgconfig.path = table.join(pkgconfig.path, { name })
end

function pkgconfig.readpc(name, extrapaths)
  if type(extrapaths) == 'string' then
    extrapaths = {extrapaths}
  end
  local paths = table.join(extrapaths or {}, pkgconfig.path)
  for i = 1, #paths do
    local p = paths[i]
    local fp = path.join(p, string.format('%s.pc', name))
	if os.isfile(fp) then
      local handle = io.open(fp)
      local rv = handle:read('*a')
      handle:close()
      return rv
    end
  end
end

function pkgconfig.parse(pc)
  local rv = {}
  local vars = {}
  for line in pc:gmatch('[^\n]+') do
    line = line:gsub("^%s*(.-)%s*$", "%1"):gsub('^#.*$', '')
    local s, e = line:find('=')
    if not s then
      s, e = line:find(':')
    end
    if s then
      k = strip(line:sub(1, s - 1)):lower()
      v = interp(strip(line:sub(e + 1, -1)), vars)
      local sep = line:sub(s, e)
      if sep == '=' then
        vars[k] = v
      elseif sep == ':' then
        rv[k] = v
      end
    end
  end
  return rv
end

function pkgconfig.load(name, extrapaths)
  local pc = pkgconfig.readpc(name, extrapaths)
  if pc then
    -- return table.tostring(pkgconfig.parse(pc))
    return pkgconfig.parse(pc)
  end
end

function pkgconfig.cflags(lib, extrapaths)
  local l = pkgconfig.load(lib, extrapaths)
  if l ~= nil then
    local cf = l.cflags
    if _TARGET_OS == 'windows' then
      cf = path.join(pkgconfig.prefix, cf)
    end
    return cf
  end
  return ''
end

function pkgconfig.libs(lib, extrapaths)
  local l = pkgconf.load(lib, extrapaths)
  if l ~= nil then
    return l.libs
  end
  return ''
end

function pkgconfig.libdir(lib, extrapaths)
  local l = pkgconf.load(lib, extrapaths)
  if l ~= nil then
    return path.join(pkgconfig.prefix, l.libdir)
  end
  return ''
end

return p.modules.pkgconfig;

