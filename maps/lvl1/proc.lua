package.path = "./?.lua;" .. package.path
require "dnglib.constants";
local default = require "dnglib.defaults"

function onKeyPress(key)
    default.onKeyPress(key)
end
