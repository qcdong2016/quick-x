
function action_spawn(actions)
    if #actions < 1 then return end
    if #actions < 2 then return actions[1] end

    local prev = actions[1]
    for i = 2, #actions do
        prev = CCSpawn:createWithTwoActions(prev, actions[i])
    end
    return prev
end

function sequence(actions)
    if #actions < 1 then return end
    if #actions < 2 then return actions[1] end

    local prev = actions[1]
    for i = 2, #actions do
        prev = CCSequence:createWithTwoActions(prev, actions[i])
    end
    return prev
end

local color_red = ccc3(233, 108, 129)
local color_green = ccc3(158, 201,  13)

local star_color = {
    purple = ccc3(227, 127, 247),
    green = ccc3(158, 201,  13),
    blue = ccc3(62, 235, 239),
    orange = ccc3(230, 195, 13),
    red = ccc3(233, 108, 129)
}


local stars = {
    "orange", "red", "purple", "blue", "green"
}

local star_size = 64
local mx = 10
local my = 10

local GameLogic = class("GameLogic", function()
    return display.newNode()
end)

local function genStarRemoveEffect(color, pos)
    local sp = display.newSprite():pos(pos.x, pos.y)

    local ani = display.newAnimation(display.newFrames("popstar/ani/ani_light_"..color.."_%d.png", 0, 7), 1/15)
    sp:playAnimationOnce(ani, true)
    return sp
end

function GameLogic:ctor(stars_image)
    self:setSize(CCSize(star_size *mx, star_size * my))

    self:setUpEvent()
    self.runner = display.newNode():addTo(self)
    self.batchnode = display.newBatchNode(stars_image):addTo(self, 9)
end

function GameLogic:enableTouch()
    self.disable_touch = false
end

function GameLogic:disableTouch()
    self.disable_touch = true
end

function GameLogic:setHook(f)
    self.hook_touch_star = f
end

function GameLogic:getRandomStar(col, row)
    local color = stars[math.random(1, #stars)]
    local x = col * star_size-star_size/2

    local star = display.newSprite("popstar/stars/"..color..".png")
        :pos(x, row * star_size - star_size/2):zorder(100)

    star.color = color
    star.i = col
    star.j = row
    star:addTo(self.batchnode)

    return star;
end

local function starEffect(star, effect, i, j)
    if effect == 1 then

        star:setOpacity(0)
        star:setScale(0)

        star:runAction(sequence({
            action_spawn({
                CCFadeTo:create(0.3, 255),
                CCScaleTo:create(0.3, 1)
            })
        }));
    elseif effect == 2 then

        star:setOpacity(0)
        star:setScale(0)
        
        star:runAction(sequence({
            CCDelayTime:create(math.random(1, 2)/10),
            action_spawn({
                CCFadeTo:create(0.3, 255),
                CCScaleTo:create(0.3, 1)
            })
        }));
    elseif effect == 3 then
        local pos = star:getPosition()

        if j % 2 == 0 then
            star:setPosition(0, pos.y)
        else
            star:setPosition(display.width, pos.y)
        end

        star:runAction(transition.newEasing(CCMoveTo:create(i/10, pos), "ELASTICOUT"))

    elseif effect == 4 then
        local pos = star:getPosition()

        star:setPosition(pos.x, 0)

        star:runAction(transition.newEasing(CCMoveTo:create(j/2/10, pos), "ELASTICOUT"))
    end
end

function GameLogic:setUpEvent()
    self:setTouchEnabled(true)

    self:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event)
        local pos = self:convertToNodeSpace(ccp(event.x, event.y))
        if event.name == 'began' then
            print(self.disable_touch )
            if not self.disable_touch then
                self:onTouchStar(pos.x, pos.y)
            end
            return true
        end
    end)
end

function GameLogic:getMap()
    local ret = {}

    for i = 1, mx do
        local stars = {}
        table.insert(ret, stars)
        for j = 1, my do
            local star = self.starTable[i][j]
            if star then
                stars[j] = star.color
            end
        end
    end
    return ret
end

function GameLogic:loadMap(map)
    math.randomseed(os.time())
    local effect = math.random(1, 4)
    local newTable = {}

    for i = 1, mx do
        local stars = {}
        table.insert(newTable, stars)
        local line = map[i]
        if line then
            for j = 1, my do
                local color = line[j]
                if color then
                    local x = i * star_size-star_size/2

                    local star = display.newSprite("#"..color..".png")
                        :pos(x, j * star_size - star_size/2):zorder(100)

                    star.color = color
                    star.i = i
                    star.j = j
                    star:addTo(self.batchnode)

                    starEffect(star, effect, i, j)
                    stars[j] = star
                end
            end
        end
    end
    return newTable
end

function GameLogic:startWithMode(map)

    if not map then
        self:genrandomMap()
    else
        local function count_star()
            if type(map) ~= 'table' then
                return false
            end
            
            for i = 1, mx do
                local line = map[i]
                if line then
                    for j = 1, my do
                        local color = line[j]
                        if color then
                            return true
                        end
                    end
                end
            end
        end

        if count_star() then
            self.starTable = self:loadMap(map)
        else
            self:genrandomMap()
        end
    end

    self:enableTouch()
end

function GameLogic:setInfo(info)
    self.onStarKill = info.onStarKill
    self.gameover = info.gameover
    self.onStepChange = info.onStepChange
    self.onItemFunc = info.onItem
end

function GameLogic:genrandomMap()
    self.starTable = {}

    math.randomseed(os.time())
    local effect = math.random(1, 4)
    for i = 1, mx do
        local t = {}

        for j = 1, my do
            local star = self:getRandomStar(i, j)
            table.insert(t, star)

            starEffect(star, effect, i, j)
        end

        table.insert(self.starTable, t)
    end
end

local function log(t)
    print('-------------')
    local tab = 
    {
        orange      = 'o',
        red         = 'r',
        purple      = 'p',
        blue        = 'b',
        green       = 'g',
    }

    local f = function (c) return (c and tab[c.color] or "*") end

    for i = 1, mx do
        for j = 1, my do
            io.write(f(t[i][j]))
            io.write(' ')
        end
        io.write('\n')
    end
    print('-------------')
end

function GameLogic:doSetColor(touched_star)
    local sprite = "#"..color..".png"

    local newStar = display.newSprite(sprite):pos(touched_star:getPosition())
                    :addTo(touched_star:getParent())

    newStar.color = color
    newStar.i = touched_star.i
    newStar.j = touched_star.j
    self.starTable[touched_star.i][touched_star.j] = newStar

    touched_star:removeSelf()
end

function GameLogic:doBomb(touched_star)
    local starTable = self.starTable

    local function get(i, j)
        local stars = starTable[i]
        if not stars then
            return nil
        else
            return stars[j]
        end
    end

    local i = touched_star.i
    local j = touched_star.j
    local bomb_tab = {}

    local function add(star) 
        if star then 
            print(star.color)
            table.insert(bomb_tab, star) 
        end 
    end

    add(touched_star)
    add(get(i, j-1))
    add(get(i, j+1))
    add(get(i-1, j))
    add(get(i-1, j-1))
    add(get(i-1, j+1))
    add(get(i+1, j+1))
    add(get(i+1, j-1))
    add(get(i+1, j))

    for i, v in ipairs(bomb_tab) do
        self:KillStar(v)
    end
end

function GameLogic:doReset()
    local alive_pos = {}
    local alive_star = {}
    local starTable = self.starTable

    for i = 1, mx do
        for j = 1, my do
            local star = starTable[i][j]
            if star then
                table.insert(alive_pos, {i=i, j=j})
                table.insert(alive_star, star)
            end
        end
    end

    for i, star in ipairs(alive_star) do
        local pos = table.remove(alive_pos, math.random(1, #alive_pos))
        local i = pos.i
        local j = pos.j
        local x = i * star_size - star_size/2
        local y = j * star_size - star_size/2

        star.i = i
        star.j = j
        starTable[i][j] = star
        star:runAction(CCMoveTo:create(0.5, ccp(x, y)))
    end
end

function GameLogic:getTheSameList(i, j)
    local starTable = self.starTable

    local function get(i, j)
        return starTable[i][j]
    end

    local star = get(i, j)

    if not star then return end

    local color = star.color

    local function check4(star)
        local ret = {}

        local i = star.i
        local j = star.j

        if i > 1 then
            local st = get(i-1, j)
            if st and st.color == color then
                table.insert(ret, st)
            end
        end

        if i < mx then
            local st = get(i+1, j)
            if st and st.color == color then
                table.insert(ret, st)
            end
        end

        if j > 1 then
            local st = get(i, j-1)
            if st and st.color == color then
                table.insert(ret, st)
            end
        end

        if j < my then
            local st = get(i, j+1)
            if st and st.color == color then
                table.insert(ret, st)
            end
        end
        return ret
    end

    local sameColorList = {}
    local checkedList   = {}
    local newchecklist  = {}
    local checklist     = {star}

    while #checklist > 0 do

        for i, star_tocheck in ipairs(checklist) do
            local forside = check4(star_tocheck)

            for j, v in ipairs(forside) do
                
                if not checkedList[v] then
                    checkedList[v] = true
                    table.insert(sameColorList, v)
                    table.insert(newchecklist, v)
                end
            end
        end

        checklist = newchecklist
        newchecklist = {}
    end

    return sameColorList
end

function GameLogic:onTouchStar(x, y)
    local i = math.ceil(x / star_size)
    local j = math.ceil(y / star_size)

    local touched_star = self.starTable[i][j]

    if touched_star then

        local pos = touched_star:getPosition()
        display.newSprite()
            :addTo(self, 99999)
            :pos(pos.x, pos.y)
            :playAnimationOnce(display.newAnimation(display.newFrames('popstar/touch/ani_touch_%02d.png', 2, 15-2), 3/60), true)

        if self.hook_touch_star and not self.hook_touch_star(touched_star) then
            return 
        end
    end

    local sameColorList = self:getTheSameList(i, j)

    if not sameColorList or #sameColorList <= 0 then
        return
    end

    for i, v in ipairs(sameColorList) do

        v:runAction(sequence({
            CCMoveTo:create(0.1, ccp(v:getPositionX(), v:getPositionY()+5)),
            CCMoveTo:create(0.1, ccp(v:getPositionX(), v:getPositionY()))
        }))
    end
    self.sameColorList = sameColorList

    self.popping = true
    self:doPoping(self.sameColorList)
    self.sameColorList = nil
end

function GameLogic:showWords(nstars)
    if nstars > 4 then
        local node = display.newNode():addTo(self, 99999):pos(display.cx, display.cy)
        local level = 1
        local sprite = display.newSprite(string.format('popstar/word_%d.png', level)):addTo(node)

        node:scale(1.5)
        node:runAction(sequence({
            CCBlink:create(0.5, 6),
            transition.newEasing(CCScaleTo:create(0.4, 1), 'ELASTICOUT'),
            CCDelayTime:create(1),
            CCRemoveSelf:create()
        }))
    end 
end

function GameLogic:doPoping(same_list)
    self:disableTouch()

    self:showWords(#same_list)

    local actions = {}
    local KillFunc = self.onStarKill

    for i, v in ipairs(same_list) do
        local this_score = i--5 + (i -1)*10

        table.insert(actions, CCDelayTime:create(0.07))
        table.insert(actions, CCCallFunc:create(function()
            self:KillStar(v, function()
                if KillFunc then
                    local pos = v:getPosition()
                    KillFunc(i, v.color, pos.x, pos.y)
                end
            end)
        end))
    end

    table.insert(actions, CCDelayTime:create(0.3))
    table.insert(actions, CCCallFunc:create(function()
        print('popping nil')
        self.popping = nil
    end))
    table.insert(actions, CCCallFunc:create(handler(self, self.shiftStars)))

    self.runner:runAction(sequence(actions))
end

function GameLogic:shiftStarsDone()
    self:shiftStars_h()
end

function GameLogic:shiftStars()
    self:disableTouch()
    self.moved_count = 0

    for i=1, mx do
        local sprites = self.starTable[i]

        for j = 1, my do
            local st = sprites[j]

            if st == nil then
                local step = j + 1
                local rep = nil

                while step <= my do
                    rep = sprites[step]
                    if rep then
                        break
                    else
                        step = step + 1
                    end
                end

                if rep then
                    rep.j = j
                    sprites[j] = rep
                    sprites[step] = nil
                    local delay = (step-j) / 10
                    
                    self.moved_count = self.moved_count + 1

                    local this_count = count
                    rep:runAction(sequence({
                        CCMoveTo:create(delay, ccp(rep:getPositionX(), j * star_size-star_size/2)),
                        CCCallFunc:create(function()
                            self.moved_count = self.moved_count - 1
                            if self.moved_count <= 0 then
                                self:shiftStarsDone()
                            end
                        end)
                    }))
                end
            end
        end
    end

    if self.moved_count <= 0 then
        self:shiftStarsDone()
    end
end

function GameLogic:shiftStars_h()
    self.moved_count = 0

    for i=1, #self.starTable do
        local sprites = self.starTable[i]
        if #sprites == 0 then
            local step = i + 1
            local rep = nil
            while step <= my do
                rep = self.starTable[step]
                if #rep ~= 0 then
                    for j, v in ipairs(rep) do
                        v.i = i
                        self.moved_count = self.moved_count + 1
                        v:runAction(sequence({
                            CCMoveTo:create((step-i)/10, ccp(i * star_size-star_size/2, j * star_size-star_size/2)),
                            CCCallFunc:create(function()
                                self.moved_count = self.moved_count - 1
                                if self.moved_count <= 0 then
                                    self:starMovedOver()
                                end
                            end)
                        }))
                    end

                    self.starTable[i] = rep
                    self.starTable[step] = sprites
                    break
                else
                    step = step + 1
                end
            end
        end
    end

    if self.moved_count <= 0 then
        self:starMovedOver()
    end
end

function GameLogic:burnAll()

    for i=1, mx do
        local sprites = self.starTable[i]
        for j=my, 1, -1 do
            local star = sprites[j]
            if star then
                self:KillStar(star)
            end
        end
    end
end

function GameLogic:KillStar(star, f)

    self.starTable[star.i][star.j] = nil

    if f then f() end

    self:addChild(genStarRemoveEffect(star.color, star:getPosition()), 999)
    star:removeSelf()
end

function GameLogic:getAliveStars()
    local t = {}
    for i=1, mx do
        local sprites = self.starTable[i]

        for j=my, 1, -1 do
            local star = sprites[j]
            if star then
                table.insert(t, star)
            end
        end
    end

    return t
end

function GameLogic:starMovedOver()
    if not self:checkEverybodyIsSingle() then
        self:enableTouch()
    else
        log(self.starTable, true)
        self.gameover()
    end
end

function GameLogic:checkEverybodyIsSingle()
    local starTable = self.starTable
    local function get(i, j)
        return starTable[i][j]
    end
                        
    for i=1, mx do
        local sprites = self.starTable[i]
        for j = 1, my do
            local star = sprites[j]

            if star then
                local color = star.color
                local i = star.i
                local j = star.j

                if i > 1 then
                    local st = get(i-1, j)
                    if st and st.color == color then
                        return false
                    end
                end

                if i < mx then
                    local st = get(i+1, j)
                    if st and st.color == color then
                        return false
                    end
                end

                if j > 1 then
                    local st = get(i, j-1)
                    if st and st.color == color then
                        return false
                    end
                end

                if j < my then
                    local st = get(i, j+1)
                    if st and st.color == color then
                        return false
                    end
                end
            end
        end
    end
    return true
end


local GamePopstar = class('GamePopstar', TestBase)

local function calcScore(count)
    return 5 + (count -1)*10
end

function GamePopstar:setup()
    self.current_level = 1
    self.current_score = nil

    self.bg = display.newSprite('popstar/back_1.jpg'):addTo(self, -1)
    self.score_label = LabelBMFont:create()
    self.score_label:setFntFile("UIFont.fnt")
    self.score_label:setPosition(ccp(0, display.cy - 100))
    self.score_label:setAnchorPoint(ccp(0.5, 0.5))
    self.score_label:addTo(self)

    self.node = display.newNode():addTo(self)

    self:performWithDelay(function()
        self:startWithMode(self.map)
        self.map = nil
    end, 0.5)
end

function GamePopstar:startWithMode(map)
    self.game = GameLogic.new("popstar/stars.png"):addTo(self)
    local sz = self.game:getSize()
    local x = - sz.width /2
    local y = - sz.height / 2
    self.game:setPosition(ccp(x, y))
    self:setCascadeBoundingBox(CCRect(x, y, sz.width, sz.height))

    local info = {}
    
    info.onStarKill = handler(self, self.onStarKill)
    info.gameover = handler(self, self.onGameOver)

    self.game:setInfo(info)
    return self:nextLevel(map)
end

function GamePopstar:nextLevel(map)
    self.current_level = self.current_level or 0
    self.current_score = self.current_score or 0

    self.completed = nil

    if (not self.target_score) or self.current_score >= self.target_score then
        self.current_level = self.current_level + 1
        self.target_score = 1000 + (self.current_level-1)*2500

        self.score_label:setText(string.format("%d/%d", 0, self.target_score))
        self.game:startWithMode(map)
    else
        -- failed
    end
end

function GamePopstar:finishTarget()

    local sp = display.newSprite('newres/word_pass.png')
            :pos(ccp(display.cx, display.top + 100))
            :addTo(self, 99)

    sp:runAction(sequence({
        transition.newEasing(CCMoveTo:create(0.5, ccp(display.cx, display.cy)), 'ELASTICOUT'),
        CCDelayTime:create(2),
        
        CCFadeTo:create(0.7, 0),
        CCRemoveSelf:create()
        }))

end

function GamePopstar:addScore(sc)
    local current_score = self.current_score + sc
    self.current_score = current_score
    self.score_label:setText(string.format("%d/%d", current_score, self.target_score))
end

function GamePopstar:onStarKill(i, color, x, y)

    local this_score = calcScore(i)

    local label = ui.newBMFontLabel({
        text = tostring(this_score),
        font = "number.fnt"
    }):addTo(self.game, 9999):pos(x, y):scale(0.7)

    local pos = self.score_label:convertToWorldSpace(ccp(0, 0))
    pos = self.game:convertToNodeSpace(pos)

    label:setColor(star_color[color])
    label:runAction(sequence({
        CCMoveTo:create(1, pos),
        CCRemoveSelf:create(),
        CCCallFunc:create(function()
            self:addScore(this_score)

            if not self.completed and self.current_score >= self.target_score then
                self.completed = true
                self:finishTarget()
            end
        end)
    }))
end

function GamePopstar:onGameOver()
    self:performWithDelay(function()
        self:calculateScore()
    end, 0.2)
end

function GamePopstar:calculateScore()
    self:enableButtons(false)

    local aliveStars = self.game:getAliveStars()
    local node = display.newNode():addTo(self, 9999):pos( display.right + 150, display.cy)

    node.label_top = Label(tostring(string.format(TEXT.LEFT_STARS_FORMAT, #aliveStars))):pos(0, 50):addTo(node)
    node.label_bottom = Label(tostring(string.format(TEXT.REWARD_FORMAT, '2000'))):addTo(node)

    node:runAction(sequence({
        CCMoveTo:create(0.5, ccp(display.cx, display.cy)),
        CCCallFunc:create(function()
            local actions = {}
            local max = math.min(10, #aliveStars)

            for idx=1, max do
                local st = table.remove(aliveStars, 1)
                local loot = 2000 - idx*idx*20
                local str = tostring((loot>=0) and loot or 0)

                table.insert(actions, CCCallFunc:create(function()
                    node.label_bottom:setString(string.format(TEXT.REWARD_FORMAT, str))
                    audio.playSound(sound.pop)
                    self.loot_socre = loot

                    self.game:KillStar(st)
                end))
                table.insert(actions, CCDelayTime:create(0.2))
            end
            
            table.insert(actions, CCDelayTime:create(0.2))
            table.insert(actions, CCCallFunc:create(function()

                if #aliveStars >= 10 then
                    node.label_bottom:setString(string.format(TEXT.REWARD_FORMAT, '0'))
                end

                if #aliveStars > 0 then
                    self.game:burnAll()
                    audio.playSound(sound.pop)
                end

                local pos_world = self.score_label:convertToWorldSpace(ccp(0,0))
                local pos_local = node.label_bottom:convertToNodeSpace(pos_world)

                node.label_bottom:runAction(sequence({
                    CCMoveTo:create(0.5, pos_local),
                    CCRemoveSelf:create(),
                    CCCallFunc:create(function()
                        self:addScore(self.loot_socre)

                        node:runAction(sequence({
                            CCDelayTime:create(1),
                            CCMoveTo:create(0.5, ccp(display.left - 150, display.cy)), 
                            CCCallFunc:create(function()
                                self:enableButtons(true)

                                node:removeSelf()
                                self:nextLevel()
                            end),
                        }))
                    end),
                }))
        
            end))   

            self:runAction(sequence(actions))
        end)
    }))
end

return GamePopstar

