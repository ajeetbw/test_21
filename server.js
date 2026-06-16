/**
 * Mock Web API Server for SVN-to-Git Testing.
 * Uses only Node.js standard libraries to remain self-contained.
 * Implements a complete RESTful router, error handling, and memory store.
 */

const http = require('http');
const url = require('url');

// --- In-Memory Database ---
const db = {
    users: [
        { id: 1, name: 'Alice Smith', role: 'Admin', active: true },
        { id: 2, name: 'Bob Jones', role: 'Developer', active: true },
        { id: 3, name: 'Charlie Brown', role: 'Viewer', active: false }
    ],
    nextId: 4
};

// --- Utility Functions ---

const sendJson = (res, statusCode, data) => {
    res.writeHead(statusCode, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify(data));
};

const parseBody = (req) => {
    return new Promise((resolve, reject) => {
        let body = '';
        req.on('data', chunk => {
            body += chunk.toString();
        });
        req.on('end', () => {
            try {
                resolve(body ? JSON.parse(body) : {});
            } catch (err) {
                reject(new Error('Invalid JSON payload'));
            }
        });
        req.on('error', reject);
    });
};

// --- Route Handlers ---

const handleGetUsers = (req, res) => {
    sendJson(res, 200, { success: true, count: db.users.length, data: db.users });
};

const handleGetUserById = (req, res, id) => {
    const user = db.users.find(u => u.id === parseInt(id, 10));
    if (user) {
        sendJson(res, 200, { success: true, data: user });
    } else {
        sendJson(res, 404, { success: false, error: 'User not found' });
    }
};

const handleCreateUser = async (req, res) => {
    try {
        const body = await parseBody(req);
        if (!body.name || !body.role) {
            return sendJson(res, 400, { success: false, error: 'Missing required fields: name, role' });
        }
        
        const newUser = {
            id: db.nextId++,
            name: body.name,
            role: body.role,
            active: body.active !== undefined ? body.active : true
        };
        
        db.users.push(newUser);
        sendJson(res, 201, { success: true, data: newUser });
    } catch (err) {
        sendJson(res, 400, { success: false, error: err.message });
    }
};

const handleDeleteUser = (req, res, id) => {
    const initialLength = db.users.length;
    db.users = db.users.filter(u => u.id !== parseInt(id, 10));
    
    if (db.users.length < initialLength) {
        sendJson(res, 200, { success: true, message: 'User deleted successfully' });
    } else {
        sendJson(res, 404, { success: false, error: 'User not found' });
    }
};

// --- Main Server Logic ---

const server = http.createServer(async (req, res) => {
    // CORS Headers
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, DELETE, OPTIONS');
    res.setHeader('Access-Control-Allow-Headers', 'Content-Type');

    if (req.method === 'OPTIONS') {
        res.writeHead(204);
        res.end();
        return;
    }

    const parsedUrl = url.parse(req.url, true);
    const path = parsedUrl.pathname;
    const method = req.method;

    console.log(`[${new Date().toISOString()}] ${method} ${path}`);

    try {
        if (path === '/api/users' && method === 'GET') {
            handleGetUsers(req, res);
        } else if (path === '/api/users' && method === 'POST') {
            await handleCreateUser(req, res);
        } else if (path.match(/^\/api\/users\/(\d+)$/) && method === 'GET') {
            handleGetUserById(req, res, path.split('/')[3]);
        } else if (path.match(/^\/api\/users\/(\d+)$/) && method === 'DELETE') {
            handleDeleteUser(req, res, path.split('/')[3]);
        } else if (path === '/health') {
            sendJson(res, 200, { status: 'UP', timestamp: Date.now() });
        } else {
            sendJson(res, 404, { success: false, error: 'Route not found' });
        }
    } catch (err) {
        console.error('Server Error:', err);
        sendJson(res, 500, { success: false, error: 'Internal Server Error' });
    }
});

const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
    console.log(`Mock Web API Server is running on port ${PORT}`);
});