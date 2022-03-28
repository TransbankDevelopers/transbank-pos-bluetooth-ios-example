//
//  ViewController.swift
//  SampleApp
//
//  Created by Developer on 04-07-18.
//  Copyright © 2018 Ingenico. All rights reserved.
//

import UIKit
import iSMP
import mPosIntegradoFrameworkiOS

class ViewController: UIViewController,ICPclServiceDelegate {
    
    var pclService = ICPclService.shared()//NEEDED
    var terminals: [ICTerminal] = []
    var utils = mPosIntegrado()
    
    var isConnected = false
    
    @IBOutlet weak var StatusLabel: UILabel!
    
    @IBOutlet weak var ResponseLabel: UILabel!
    @IBOutlet weak var ResponseTextView: UITextView!
    
    @IBOutlet weak var togleConnectionButton: UIButton!
    
    @IBOutlet weak var amountTextField: UITextField!
    @IBOutlet weak var opNumberTextField: UITextField!
    
    var ssl: ICSSLParameters = {
        let initValue = ICSSLParameters()
        initValue.isSSL=false
        initValue.sslCertificateName = "serverb"
        initValue.sslCertificatePassword = "coucou"
        return initValue
    }()//TSL CONNECTION ON TERMINALS TO BE IMPLEMENTED FOR THE FINAL RELEASE

    override func viewDidLoad() {
        super.viewDidLoad()
        self.pclService?.delegate=self
        utils.onFinishTransaction =
            {
                result in
                // DO SOMETHING WITH THE RESPONSE
                self.processMessage(message: result)
                // DO SOMETHING WITH THE RESPONSE
                
        }//NEEDED TO CAPTURE RESULT OF TRANSACTION
        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(true)
        self.pclService?.stop()
        self.pclService?.delegate = nil
    }
    
    func startPclService(terminal:ICTerminal, sslParameters: ICSSLParameters) -> pclServiceState?
    {
        pclService?.delegate=self
        return pclService?.start(with: terminal, andSecurity: sslParameters)
    }
    
    func SelecTerminalAndStartPCLDemo()
    {
        var terminals: [ICTerminal] = []
        utils.setPclServiceforUtils(service: self.pclService!)
        self.pclService?.stop()
        terminals=utils.getDevices();
        if(terminals != [] )
        {
            let terminalselected = terminals[0]
            
            if(self.startPclService(terminal: terminalselected, sslParameters: self.ssl ) != PCL_SERVICE_STARTED) {
                Toast.show(message: "No se pudo conectar al POS", controller: self)
            }
        }
        else
        {
            Toast.show(message: "No se encontro ningún POS", controller: self)
        }
    }
    
    public func notifyConnection(_ sender: ICPclService!)
    {
        //DO SOMETHING WHEN THE SERVER IS CONNECTED
        StatusLabel.text = "Conectado"
        StatusLabel.textColor = UIColor.systemGreen
        togleConnectionButton.setTitle("Desconectar", for: .normal)
        isConnected = true
    }
    
    public func notifyDisconnection(_ sender: ICPclService!)
    {
        //DO SOMETHING WHEN THE SERVER IS DISCONNECTED
        StatusLabel.text = "Desconectado"
        StatusLabel.textColor = UIColor.systemRed
        togleConnectionButton.setTitle("Conectar", for:.normal)
        isConnected = false
    }
    
    /*Este callback se usa para mostrar un log en consola del progreso*/
    public func pclLogEntry(_ message: String!, withSeverity severity: Int32)
    {
        print("\(ICPclService.severityLevelString(severity) ?? "") \(message ?? "")")
    }
    
    @IBAction func togleConnection(_ sender: UIButton) {
        if (!isConnected) {
            self.SelecTerminalAndStartPCLDemo()
            return
        }
        
        StatusLabel.text = "Desconectado"
        StatusLabel.textColor = UIColor.systemRed
        togleConnectionButton.setTitle("Conectar", for:.normal)
        self.pclService?.stop()
    }
        
    @IBAction func loadKeys(_ sender: UIButton)
    {
        if(terminalIsConnected())
        {
            sendToPOS(command: "0800")
        }
    }
    
    @IBAction func lastSale(_ sender: UIButton)
    {
        if(terminalIsConnected())
        {
            sendToPOS(command: "0250|0")
        }
    }
    
    @IBAction func totals(_ sender: UIButton)
    {
        if(terminalIsConnected())
        {
            sendToPOS(command: "0700||")
        }
    }
    
    @IBAction func close(_ sender: UIButton)
    {
        if(terminalIsConnected())
        {
            sendToPOS(command: "0500|0")
        }
    }
    
    @IBAction func details(_ sender: UIButton)
    {
        if(terminalIsConnected())
        {
            sendToPOS(command: "0260|1")
        }
    }
    
    @IBAction func sale(_ sender: UIButton)
    {
        let amount = Int(amountTextField.text ?? "") ?? 0
        
        if(amount < 50) {
            Toast.show(message: "El monto debe ser mayor o igual a $50", controller: self)
            return
        }
        
        if(amount > 999999999) {
            Toast.show(message: "El monto debe ser menor o igual a $999.999.999", controller: self)
            return
        }
        
        if(terminalIsConnected())
        {
            sendToPOS(command: "0200|\(amount)|123456|||0")
        }
    }
    
    @IBAction func refund(_ sender: UIButton)
    {
        let operationNumber = Int(opNumberTextField.text ?? "") ?? 0
        
        if(operationNumber > 0) {
            Toast.show(message: "El número de operación debe ser mayor a 0", controller: self)
            return
        }
        
        if(operationNumber > 999999) {
            Toast.show(message: "El número de operación debe ser menor o igual a 999999", controller: self)
            return
        }
        
        if(terminalIsConnected())
        {
            sendToPOS(command: "1200|\(operationNumber)|")
        }
    }
    
    func terminalIsConnected() -> Bool
    {
        if(pclService?.getState() == PCL_SERVICE_CONNECTED) {
            return true
        }
        
        Toast.show(message: "POS no conectado", controller: self)
        return false
    }
    
    func processMessage(message: String)
    {
        let asciiResponse = hexStringToAscii(hexString: message)
        ResponseTextView.text = asciiResponse
        
        print("Hex response: \(message)")
        print("ASCII response: \(asciiResponse)")
    }
    
    func sendToPOS(command: String) {
        let STX = String(UnicodeScalar(0x02))
        let ETX = String(UnicodeScalar(0x03))
        
        let partialCommand = STX + command + ETX
        let lrc = calculateLRC(command: partialCommand)
        let fullCommand = partialCommand + String(UnicodeScalar(lrc))
        let hexCommand = fullCommand.toHexEncodedString()
        
        utils.startTransaction(payload: hexCommand)
    }
    
    func calculateLRC(command: String) -> UInt8 {
        var lrc = UInt8()
        let range = command.index(after: command.startIndex)..<command.endIndex
        
        for char in command[range].utf8 {
            lrc ^= char
        }
        
        return lrc
    }
    
    func hexStringToAscii(hexString: String) -> String {
        let pattern = "(0x)?([0-9a-f]{2})"
        let regex = try! NSRegularExpression(pattern: pattern, options: .caseInsensitive)
        let nsString = hexString as NSString
        let matches = regex.matches(in: hexString, options: [], range: NSMakeRange(0, nsString.length))
        let characters = matches.map {
           Character(UnicodeScalar(UInt32(nsString.substring(with: $0.range(at: 2)), radix: 16)!)!)
        }
        return String(characters)
    }
    
    class Toast {
        static func show(message: String, controller: UIViewController) {
            let toastContainer = UIView(frame: CGRect())
            toastContainer.backgroundColor = UIColor.black.withAlphaComponent(0.6)
            toastContainer.alpha = 0.0
            toastContainer.layer.cornerRadius = 25;
            toastContainer.clipsToBounds  =  true
            
            let toastLabel = UILabel(frame: CGRect())
            toastLabel.textColor = UIColor.white
            toastLabel.textAlignment = .center;
            toastLabel.font.withSize(12.0)
            toastLabel.text = message
            toastLabel.clipsToBounds  =  true
            toastLabel.numberOfLines = 0
            
            toastContainer.addSubview(toastLabel)
            controller.view.addSubview(toastContainer)
            
            toastLabel.translatesAutoresizingMaskIntoConstraints = false
            toastContainer.translatesAutoresizingMaskIntoConstraints = false
            
            let a1 = NSLayoutConstraint(item: toastLabel, attribute: .leading, relatedBy: .equal, toItem: toastContainer, attribute: .leading, multiplier: 1, constant: 15)
            let a2 = NSLayoutConstraint(item: toastLabel, attribute: .trailing, relatedBy: .equal, toItem: toastContainer, attribute: .trailing, multiplier: 1, constant: -15)
            let a3 = NSLayoutConstraint(item: toastLabel, attribute: .bottom, relatedBy: .equal, toItem: toastContainer, attribute: .bottom, multiplier: 1, constant: -15)
            let a4 = NSLayoutConstraint(item: toastLabel, attribute: .top, relatedBy: .equal, toItem: toastContainer, attribute: .top, multiplier: 1, constant: 15)
            toastContainer.addConstraints([a1, a2, a3, a4])
            
            let c1 = NSLayoutConstraint(item: toastContainer, attribute: .leading, relatedBy: .equal, toItem: controller.view, attribute: .leading, multiplier: 1, constant: 65)
            let c2 = NSLayoutConstraint(item: toastContainer, attribute: .trailing, relatedBy: .equal, toItem: controller.view, attribute: .trailing, multiplier: 1, constant: -65)
            let c3 = NSLayoutConstraint(item: toastContainer, attribute: .bottom, relatedBy: .equal, toItem: controller.view, attribute: .bottom, multiplier: 1, constant: -75)
            controller.view.addConstraints([c1, c2, c3])
            
            UIView.animate(withDuration: 0.5, delay: 0.0, options: .curveEaseIn, animations: {
                toastContainer.alpha = 1.0
            }, completion: { _ in
                UIView.animate(withDuration: 0.5, delay: 1.5, options: .curveEaseOut, animations: {
                    toastContainer.alpha = 0.0
                }, completion: {_ in
                    toastContainer.removeFromSuperview()
                })
            })
        }
    }
}

extension String {
    func toHexEncodedString(uppercase: Bool = true, prefix: String = "", separator: String = "") -> String {
        let format = uppercase ? "%02X" : "%02x"
        return unicodeScalars.map { prefix + .init(format: format, $0.value) } .joined(separator: separator)
    }
}
