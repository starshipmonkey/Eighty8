      target.centerX = biggestFace->x + (biggestFace->w / 2);
      target.centerX = highestScorePerson->x + (highestScorePerson->w / 2);
void updateTargetFromReceiver() {
  currentTarget = getHighestPriorityTarget();
  
  // Debug output for centerX calculation
  if (currentTarget.isValid) {
    Serial.println("Debug - Target data:");
    Serial.print("  Original X: "); Serial.print(currentTarget.x);
    Serial.print(", Width: "); Serial.print(currentTarget.width);
    Serial.print(", Calculated centerX: "); Serial.println(currentTarget.centerX);
    
    // Validate center calculation
    int expectedCenter = currentTarget.x + (currentTarget.width / 2);
    if (currentTarget.centerX != expectedCenter) {
      Serial.print("  WARNING: centerX mismatch. Expected: "); 
      Serial.print(expectedCenter);
      Serial.print(", Actual: "); 
      Serial.println(currentTarget.centerX);
      // Correct if there's a mismatch
      currentTarget.centerX = expectedCenter;
    }
  }
}
