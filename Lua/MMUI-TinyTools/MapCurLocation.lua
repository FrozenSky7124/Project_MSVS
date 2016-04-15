WorldMapButton:HookScript("OnUpdate", function(self) 
   if not self.coordText then 
      self.coordText = WorldMapFrameCloseButton:CreateFontString(nil, "OVERLAY", "GameFontGreen") 
      self.coordText:SetPoint("BOTTOM", self, "BOTTOM", 0, 6) 
   end 
   local px, py = GetPlayerMapPosition("player") 
   local x, y = GetCursorPosition() 
   local width, height, scale = self:GetWidth(), self:GetHeight(), self:GetEffectiveScale() 
   local centerX, centerY = self:GetCenter() 
   x, y = (x/scale - (centerX - (width/2))) / width, (centerY + (height/2) - y/scale) / height 
   if px == 0 and py == 0 and (x > 1 or y > 1 or x < 0 or y < 0) then 
      self.coordText:SetText("") 
   elseif px == 0 and py == 0 then 
      self.coordText:SetText(format("当前: %d, %d", x*100, y*100)) 
   elseif x > 1 or y > 1 or x < 0 or y < 0 then 
      self.coordText:SetText(format("玩家: %d, %d", px*100, py*100)) 
   else 
      self.coordText:SetText(format("玩家: %d, %d 当前: %d, %d", px*100, py*100, x*100, y*100)) 
   end 
end)