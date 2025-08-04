🗳️ Biometric Voting System

A microcontroller-based biometric voting system that uses fingerprint verification to authenticate voters and securely record their votes, reducing fraud and ensuring election integrity.

🔐 Features
    
    Biometric Authentication: Validates voters using fingerprint recognition to prevent impersonation and duplicate voting.
    
    Digital Voting Interface: Simple user interface for casting votes after successful authentication.
    
    Secure Vote Storage: Votes are securely stored to maintain integrity and confidentiality.
    
    Admin Control Panel: Optional admin functionality for monitoring and system management.
    
    Audit Trail: Logs all events for verification and transparency.

🧰 Tech Stack
    
    Microcontroller: STM32F401RBT6 
    
    Biometric Module: GT521F32 Fingerprint Sensor
    
    Programming Language: C (Embedded)
    
    Display: 16X2 Monochrome LCD 
    
    Storage: AT24C02 EEPROM 

📌 How It Works
    
    Voter places their finger on the fingerprint sensor.
    
    System compares fingerprint with the registered database.
    
    If verified, voting interface is unlocked.
    
    Voter selects a candidate and vote is securely stored.
    
    System logs the transaction for future auditing.

🚀 Future Enhancements
   
    Add facial recognition as a second layer of authentication.
    
    Integrate with blockchain for immutable vote storage.
    
    Web-based admin dashboard for real-time monitoring.
