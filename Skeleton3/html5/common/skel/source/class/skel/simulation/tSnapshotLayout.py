import tSnapshot
import Util
import time
import unittest
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.action_chains import ActionChains

#Test that a layout snapshot can be saved/restored.
class tSnapshotLayout(tSnapshot.tSnapshot):
        
    # Take a snapshot of the analysis layout.  Change to an image layout.  Restore
    # the analysis layout
    def test_analysis_saveRestore(self):    
        driver = self.driver
        time.sleep(5)
 
        #For later use, determine the number of DisplayWindows.
        windowList = driver.find_elements_by_xpath("//div[@qxclass='skel.widgets.Window.DisplayDesktop']")
        windowCount = len( windowList )
        print "Window Count=", windowCount
        time.sleep(2)
        
        # Find the session button on the menu bar and click it.
        self._clickSessionButton( driver )
        
        # Find the save session button in the submenu and click it.
        self._clickSessionSaveButton( driver )
        
        # The save popup should be visible.  Make sure preferences and data are not checked;
        # layout is checked.
        self._setSaveOptions( driver, False, True, False)
        
        # Type in tSnapshotLayout for the save name.
        self._setSaveName( driver, "tSnapshotLayout")
        
        # Hit the save button
        self._saveSnapshot( driver )
        
        # Close the dialog
        self._closeSave( driver )
        
        # Change to an image layout
        layoutButton = driver.find_element_by_xpath("//div[text()='Layout']/..")
        self.assertIsNotNone( layoutButton, "Could not find div with text layout")
        ActionChains(driver).click(layoutButton).perform()
        imageLayoutButton = driver.find_element_by_xpath( "//div[text()='Image Layout']/..")
        self.assertIsNotNone( imageLayoutButton, "Could not find layout image button in submenu")
        ActionChains(driver).click( imageLayoutButton).perform()
        
        # Click the restore sessions button
        self._clickSessionButton( driver )
        self._clickSessionRestoreButton( driver )
        
        # Select tSnapshotLayout in the combo box
        self._selectRestoreSnapshot( driver, "tSnapshotLayout")
        
        # Hit the restore button
        self._restoreSnapshot( driver )
        
        # Close the restore dialog
        self._closeRestore( driver )
        time.sleep(2)

        # Verify the window count is the same
        newWindowList = driver.find_elements_by_xpath("//div[@qxclass='skel.widgets.Window.DisplayDesktop']")
        newWindowCount = len( newWindowList )
        print "New Window Count=", newWindowCount
        self.assertEqual( windowCount, newWindowCount, "Window count changed with restore")

if __name__ == "__main__":
    unittest.main()   